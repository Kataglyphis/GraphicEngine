#include "LightingPass.h"

LightingPass::LightingPass()
{
}

void LightingPass::init(std::shared_ptr<LightingPassShaderProgram> shader_program)
{

    this->uniform_helper = UniformHelper();
    this->shader_program = shader_program;
    current_offset = glm::vec3(0.0f);

    quad.init();

    random_numbers = RandomNumbers();

    random_number_data = random_numbers.generate_random_numbers();

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

void LightingPass::execute( glm::mat4 projection_matrix, glm::mat4 view_matrix, std::shared_ptr<GBuffer> gbuffer, 
                            std::shared_ptr<DirectionalLight> main_light, 
                            std::vector<std::shared_ptr<PointLight>>& point_lights, GLuint point_light_count, 
                            glm::vec3 camera_position,
                            std::vector<ObjMaterial> materials, std::shared_ptr<Noise> noise, 
                            std::shared_ptr<Clouds> cloud, float delta_time)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_program->use_shader_program();
    retrieve_lighting_pass_locations(   projection_matrix, view_matrix, gbuffer, main_light, 
                                        point_lights, point_light_count,
                                        camera_position,
                                        materials, cloud, delta_time);

    bind_buffers_for_lighting(gbuffer, main_light, noise, point_light_count, cloud);

    quad.render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightingPass::retrieve_lighting_pass_locations(glm::mat4 projection_matrix, glm::mat4 view_matrix, std::shared_ptr<GBuffer> gbuffer,
                                                    std::shared_ptr<DirectionalLight> main_light, 
                                                    std::vector<std::shared_ptr<PointLight>>& point_lights, GLuint point_light_count,
                                                    glm::vec3 camera_position,
                                                    std::vector<ObjMaterial> materials, 
                                                    std::shared_ptr<Clouds> cloud, float delta_time)
{
    DirectionalLightUniformLocations d_light_uniform_locations;

    d_light_uniform_locations.uniform_ambient_intensity_location    = shader_program->get_directional_light_ambient_intensity_location();
    d_light_uniform_locations.uniform_color_location                = shader_program->get_directional_light_color_location();
    d_light_uniform_locations.uniform_diffuse_intensity_location    = shader_program->get_directional_light_diffuse_intensity_location();
    d_light_uniform_locations.uniform_direction_location            = shader_program->get_directional_light_direction_location();

    uniform_helper.setUniformFloat(main_light->get_ambient_intensity(), d_light_uniform_locations.uniform_ambient_intensity_location);
    uniform_helper.setUniformFloat(main_light->get_diffuse_intensity(), d_light_uniform_locations.uniform_diffuse_intensity_location);
    uniform_helper.setUniformVec3(main_light->get_color(), d_light_uniform_locations.uniform_color_location);
    uniform_helper.setUniformVec3(main_light->get_direction(), d_light_uniform_locations.uniform_direction_location);

    std::vector<GLfloat> cascade_slots = main_light->get_cascaded_slots();

    for (int i = 0; i < NUM_CASCADES; i++) {


    }

    // EVERYTHING REGARDING THE SHADOW CASCADE
    glm::mat4 light_view = main_light->get_light_view_matrix();
    std::vector<glm::mat4> cascade_light_matrices = main_light->get_cascaded_light_matrices();
    for (size_t i = 0; i < NUM_CASCADES; i++) {

        glm::vec4 clip_end_slot = projection_matrix * glm::vec4(0.0f, 0.0f, -cascade_slots[i+1], 1.0f);
        uniform_helper.setUniformFloat(clip_end_slot.z, shader_program->get_cascade_endpoint_location(i));

        uniform_helper.setUniformInt(   D_LIGHT_SHADOW_TEXTURES_SLOT + i, 
                                        shader_program->get_directional_shadow_map_location(i));

        uniform_helper.setUniformMatrix4fv( cascade_light_matrices[i] * light_view,
                                            shader_program->get_directional_light_transform_location(i));
    }

    // GBUFFER
    gbuffer->read(  shader_program->get_g_position_location(),
                    shader_program->get_g_normal_location(),
                    shader_program->get_g_albedo_location(),
                    shader_program->get_g_frag_depth_location(),
                    shader_program->get_uniform_material_id_location());


    //shader_program->set_point_lights(point_lights, P_LIGHT_SHADOW_TEXTURES_SLOT, 0);

    //shader_program->set_noise_textures(WORLEY_NOISE_TEXTURES_SLOT);

    //shader_program->set_cloud_texture(CLOUD_TEXTURE_SLOT);

    // CAMERA
    uniform_helper.setUniformVec3(camera_position, shader_program->get_eye_position_location());

    // MATERIALS
    for (size_t i = 0; i < materials.size(); i++) {

        materials[i].use_material(shader_program->get_uniform_material_locations(i));

    }

    // CLOUDS
    uniform_helper.setUniformVec3(cloud->get_rad(), shader_program->get_uniform_cloud_rad_location());

    GLfloat velocity = cloud->get_movement_speed() * delta_time;
    current_offset = current_offset + cloud->get_movement_direction() * velocity;
    uniform_helper.setUniformVec3(current_offset, shader_program->get_uniform_cloud_offset());

    uniform_helper.setUniformMatrix4fv(cloud->get_model(), shader_program->get_uniform_cloud_model());

    uniform_helper.setUniformFloat(cloud->get_scale(), shader_program->get_uniform_cloud_scale_location());
    uniform_helper.setUniformFloat(1.f - cloud->get_density(), shader_program->get_uniform_cloud_threshold_location());
    uniform_helper.setUniformFloat(cloud->get_pillowness(), shader_program->get_uniform_pillowness_location());
    uniform_helper.setUniformFloat(cloud->get_cirrus_effect(), shader_program->get_cirrus_effect_location());

    if (cloud->get_powder_effect()) {
        uniform_helper.setUniformInt(true, shader_program->get_cloud_powderness_effect());
    }
    else {
        uniform_helper.setUniformInt(false, shader_program->get_cloud_powderness_effect());
    }

    GLuint num_active_slots = main_light->get_shadow_map()->get_num_active_cascades();
    uniform_helper.setUniformInt(num_active_slots, shader_program->get_uniform_num_active_cascades_location());
    uniform_helper.setUniformInt(   main_light->get_shadow_map()->get_pcf_radius(), 
                                    shader_program->get_uniform_pcf_radius_location());
    uniform_helper.setUniformFloat( main_light->get_shadow_map()->get_intensity(), 
                                    shader_program->get_directional_light_shadow_intensity_location());

    shader_program->validate_program();
}

void LightingPass::bind_buffers_for_lighting(std::shared_ptr<GBuffer> gbuffer, std::shared_ptr<DirectionalLight> main_light, std::shared_ptr<Noise> noise, GLuint point_light_count, std::shared_ptr<Clouds> cloud)
{

    main_light->get_shadow_map()->read(D_LIGHT_SHADOW_TEXTURES_SLOT);

    cloud->read(CLOUD_TEXTURE_SLOT);

    noise->read_worley_noise(WORLEY_NOISE_TEXTURES_SLOT);

    noise->read_grad_noise(GRAD_NOISE_TEXTURES_SLOT);

    bind_random_numbers(RANDOM_NUMBERS_SLOT);

}

void LightingPass::bind_random_numbers(GLuint texture_unit)
{

    uniform_helper.setUniformInt(texture_unit, shader_program->get_random_number_location());

    glActiveTexture(GL_TEXTURE0 + (GLenum)texture_unit);
    glBindTexture(GL_TEXTURE_2D, random_number);

}


LightingPass::~LightingPass()
{

    glDeleteTextures(1, &random_number);

}