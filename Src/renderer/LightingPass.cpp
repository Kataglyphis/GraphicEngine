#include "LightingPass.h"

LightingPass::LightingPass()
{
}

void LightingPass::init(std::shared_ptr<LightingPassShaderProgram> shader_program)
{

    this->shader_program = shader_program;
    current_offset = glm::vec3(0.0f);

    quad.init();

    generate_random_numbers();

    glGenTextures(1, &random_number);
    glBindTexture(GL_TEXTURE_2D, random_number);
    // i think we won't need nearest option; so stick to linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //assuming full HD will be maximum resolution 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, MAX_RESOLUTION_X, MAX_RESOLUTION_Y, 0, GL_RGBA, GL_FLOAT, random_number_data.get());
    glBindTexture(GL_TEXTURE_2D, 0);

}

void LightingPass::execute(glm::mat4 projection_matrix, glm::mat4 view_matrix, std::shared_ptr<GBuffer> gbuffer, std::shared_ptr<DirectionalLight> main_light, 
                                                    std::vector<std::shared_ptr<PointLight>>& point_lights, GLuint point_light_count, glm::vec3 camera_position, GLuint material_counter,
                                                    std::vector<std::shared_ptr<Material>>& materials, std::shared_ptr<Noise> noise, std::shared_ptr<Clouds> cloud, float delta_time)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_program->use_shader_program();
    retrieve_lighting_pass_locations(projection_matrix, view_matrix, gbuffer, main_light, 
                                                                point_lights, point_light_count,
                                                                camera_position, material_counter,
                                                              materials, cloud, delta_time);

    bind_buffers_for_lighting(gbuffer, main_light, noise, point_light_count, cloud);

    quad.render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightingPass::generate_random_numbers()
{
    random_number_data = std::shared_ptr<GLfloat[]>(new GLfloat[MAX_RESOLUTION_X * MAX_RESOLUTION_Y * 4]); // we only store one float in the red channel

    std::mt19937_64 gen64(25121995);
    std::uniform_real_distribution<float> dis(0, 1);

    for (int i = 0; i < MAX_RESOLUTION_X; i++) {

        for (int k = 0; k < MAX_RESOLUTION_Y; k++) {
            
            GLfloat random_offset[4] = { dis(gen64), dis(gen64), dis(gen64), dis(gen64) };

            GLuint index = (MAX_RESOLUTION_Y * i + k) * 4;

            *(random_number_data.get() + index) = random_offset[0];//random_offset[0];//
            *(random_number_data.get() + index + 1) = random_offset[1];//
            *(random_number_data.get() + index + 2) = random_offset[2];//
            *(random_number_data.get() + index + 3) = random_offset[3];//
        }
    }
}

void LightingPass::retrieve_lighting_pass_locations(glm::mat4 projection_matrix, glm::mat4 view_matrix, std::shared_ptr<GBuffer> gbuffer,
                                                                       std::shared_ptr<DirectionalLight> main_light, std::vector<std::shared_ptr<PointLight>>& point_lights, GLuint point_light_count,
                                                                         glm::vec3 camera_position, GLuint material_counter,
                                                                          std::vector<std::shared_ptr<Material>>& materials, std::shared_ptr<Clouds> cloud, float delta_time)
{
    DirectionalLightUniformLocations d_light_uniform_locations;

    d_light_uniform_locations.uniform_ambient_intensity_location = shader_program->get_directional_light_ambient_intensity_location();
    d_light_uniform_locations.uniform_color_location = shader_program->get_directional_light_color_location();
    d_light_uniform_locations.uniform_diffuse_intensity_location = shader_program->get_directional_light_diffuse_intensity_location();
    d_light_uniform_locations.uniform_direction_location = shader_program->get_directional_light_direction_location();

    glUniform1f(d_light_uniform_locations.uniform_ambient_intensity_location, main_light->get_ambient_intensity());
    glUniform1f(d_light_uniform_locations.uniform_diffuse_intensity_location, main_light->get_diffuse_intensity());
    glUniform3f(d_light_uniform_locations.uniform_color_location, main_light->get_color().x, main_light->get_color().y, main_light->get_color().z);
    glUniform3f(d_light_uniform_locations.uniform_direction_location, -main_light->get_direction().x, -main_light->get_direction().y, -main_light->get_direction().z);

    std::vector<GLfloat> cascade_slots = main_light->get_cascaded_slots();

    for (int i = 0; i < NUM_MAX_CASCADES; i++) {

        glm::vec4 clip_end_slot = projection_matrix * glm::vec4(0.0f, 0.0f, -cascade_slots[i+1], 1.0f);
        glUniform1f(shader_program->get_cascade_endpoint_location(i), clip_end_slot.z);

    }

    GLuint g_buffer_lighting_uniform_directional_shadow_map_locations[NUM_MAX_CASCADES];

    for (size_t i = 0; i < NUM_MAX_CASCADES; i++) {

        g_buffer_lighting_uniform_directional_shadow_map_locations[i] = shader_program->get_directional_shadow_map_location(i);

    }

    gbuffer->use_gbuffer(shader_program->get_g_position_location(),
        shader_program->get_g_normal_location(),
        shader_program->get_g_albedo_location(),
        shader_program->get_g_frag_depth_location(),
        shader_program->get_uniform_material_id_location());

    for (size_t i = 0; i < NUM_MAX_CASCADES; i++) {

        glUniform1i(g_buffer_lighting_uniform_directional_shadow_map_locations[i], G_BUFFER_SIZE + 1 + i);

    }

    GLuint num_active_slots = main_light->get_shadow_map()->get_num_active_cascades();

    shader_program->set_point_lights(point_lights, point_light_count, G_BUFFER_SIZE + NUM_MAX_CASCADES + 1, 0);

    shader_program->set_noise_textures(G_BUFFER_SIZE + point_light_count + NUM_MAX_CASCADES + 1);

    shader_program->set_cloud_texture((GLenum)G_BUFFER_SIZE + NUM_MAX_CASCADES + point_light_count + NUM_NOISE_TEXTURES + 1);

    glUniform3f(shader_program->get_eye_position_location(), camera_position.x, camera_position.y, camera_position.z);


    for (size_t i = 0; i < material_counter; i++) {

        materials[i]->use_material( shader_program->get_uniform_material_metallic_location(i),
                                    shader_program->get_uniform_material_roughness_location(i),
                                    shader_program->get_uniform_IOR_location(i), 
                                    shader_program->get_uniform_absorption_location(i));

    }

    glUniform3f(shader_program->get_uniform_cloud_rad_location(), cloud->get_rad().x, cloud->get_rad().y, cloud->get_rad().z);

    GLfloat velocity = cloud->get_movement_speed() * delta_time;
    current_offset = current_offset + cloud->get_movement_direction() * velocity;

    glUniform3f(shader_program->get_uniform_cloud_offset(), current_offset.x, current_offset.y, current_offset.z);

    glUniformMatrix4fv(shader_program->get_uniform_cloud_model(), 1, GL_FALSE, glm::value_ptr(cloud->get_model()));

    glUniform1f(shader_program->get_uniform_cloud_scale_location(), cloud->get_scale());
    //we want to set the threshold, so 1.f - density !
    glUniform1f(shader_program->get_uniform_cloud_threshold_location(), 1.f - cloud->get_density());

    glUniform1f(shader_program->get_uniform_pillowness_location(), cloud->get_pillowness());

    glUniform1f(shader_program->get_cirrus_effect_location(), cloud->get_cirrus_effect());

    if (cloud->get_powder_effect()) {
        glUniform1i(shader_program->get_cloud_powderness_effect(), true);
    }
    else {
        glUniform1i(shader_program->get_cloud_powderness_effect(), false);
    }

    glm::mat4 light_view = main_light->get_light_view_matrix();

    std::vector<glm::mat4> cascade_light_matrices = main_light->get_cascaded_light_matrices();

    for (int i = 0; i < NUM_MAX_CASCADES; i++) {

        glUniformMatrix4fv(shader_program->get_directional_light_transform_location(i), 1, GL_FALSE, glm::value_ptr(cascade_light_matrices[i] * light_view));

    }

    glUniform1i(shader_program->get_uniform_num_active_cascades_location(), num_active_slots);

    glUniform1i(shader_program->get_uniform_pcf_radius_location(), main_light->get_shadow_map()->get_pcf_radius());

    glUniform1f(shader_program->get_directional_light_shadow_intensity_location(), main_light->get_shadow_map()->get_intensity());

    shader_program->validate_program();
}

void LightingPass::bind_buffers_for_lighting(std::shared_ptr<GBuffer> gbuffer, std::shared_ptr<DirectionalLight> main_light, std::shared_ptr<Noise> noise, GLuint point_light_count, std::shared_ptr<Clouds> cloud)
{

    //GLuint num_active_slots = main_light->get_shadow_map()->get_num_active_cascades();

    GLuint start_texture = 1;

    gbuffer->read(start_texture);

    main_light->get_shadow_map()->read((GLenum)G_BUFFER_SIZE + start_texture);

    cloud->read((GLenum)G_BUFFER_SIZE + NUM_MAX_CASCADES + point_light_count + NUM_NOISE_TEXTURES + 1);

    noise->read_worley_noise((GLenum)G_BUFFER_SIZE + NUM_MAX_CASCADES + point_light_count + 1);

    noise->read_grad_noise((GLenum)G_BUFFER_SIZE + NUM_MAX_CASCADES + point_light_count + 2);

    bind_random_numbers(G_BUFFER_SIZE + NUM_MAX_CASCADES + point_light_count + NUM_NOISE_TEXTURES + NUM_CLOUDS + 1);

}

void LightingPass::bind_random_numbers(GLuint texture_unit)
{

    glUniform1i(shader_program->get_random_number_location(), texture_unit);

    glActiveTexture(GL_TEXTURE0 + (GLenum)texture_unit);
    glBindTexture(GL_TEXTURE_2D, random_number);

}


LightingPass::~LightingPass()
{

    glDeleteTextures(1, &random_number);

}