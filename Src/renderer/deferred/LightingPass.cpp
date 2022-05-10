#include "LightingPass.h"

LightingPass::LightingPass()
{
}

void LightingPass::init()
{

    this->uniform_helper = UniformHelper();
    create_shader_program();
    current_offset = glm::vec3(0.0f);

    quad.init();

}

void LightingPass::execute( glm::mat4 projection_matrix, 
                            std::shared_ptr<Camera> main_camera,
                            std::shared_ptr<Scene> scene,
                            std::shared_ptr<GBuffer> gbuffer,
                            float delta_time)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::shared_ptr<DirectionalLight>           main_light          = scene->get_sun();
    std::vector<std::shared_ptr<PointLight>>    point_lights        = scene->get_point_lights();
    std::shared_ptr<Clouds>                     cloud               = scene->get_clouds();
    GLuint                                      point_light_count   = scene->get_point_light_count();
    glm::vec3                                   camera_position     = main_camera->get_camera_position();

    shader_program->use_shader_program();
    retrieve_lighting_pass_locations(   projection_matrix, main_camera, 
                                        scene, gbuffer,
                                        delta_time);

    bind_buffers_for_lighting(gbuffer, scene, cloud);

    quad.render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightingPass::create_shader_program()
{
    shader_program = std::make_shared<LightingPassShaderProgram>(LightingPassShaderProgram{});
    shader_program->create_from_files(  "rasterizer/g_buffer_lighting_pass.vert",
                                        "rasterizer/g_buffer_lighting_pass.frag");
}

void LightingPass::retrieve_lighting_pass_locations(glm::mat4 projection_matrix, 
                                                    std::shared_ptr<Camera> main_camera,
                                                    std::shared_ptr<Scene> scene,
                                                    std::shared_ptr<GBuffer> gbuffer,
                                                    float delta_time)
{

    glm::mat4 view_matrix = main_camera->calculate_viewmatrix();
    uniform_helper.setUniformMatrix4fv(view_matrix, shader_program->get_uniform_view_location());
    uniform_helper.setUniformMatrix4fv(projection_matrix, shader_program->get_uniform_projection_location());

    DirectionalLightUniformLocations d_light_uniform_locations;

    d_light_uniform_locations.uniform_radiance_location             = shader_program->get_directional_light_radiance_location();
    d_light_uniform_locations.uniform_color_location                = shader_program->get_directional_light_color_location();
    d_light_uniform_locations.uniform_direction_location            = shader_program->get_directional_light_direction_location();

    std::shared_ptr<DirectionalLight> main_light = scene->get_sun();
    uniform_helper.setUniformFloat(main_light->get_radiance(), d_light_uniform_locations.uniform_radiance_location);
    uniform_helper.setUniformVec3(main_light->get_color(), d_light_uniform_locations.uniform_color_location);
    uniform_helper.setUniformVec3(main_light->get_direction(), d_light_uniform_locations.uniform_direction_location);

    // EVERYTHING REGARDING THE SHADOW CASCADE
    glm::mat4 light_view = main_light->get_light_view_matrix();
    std::vector<glm::mat4> cascade_light_matrices = main_light->get_cascaded_light_matrices();
    uniform_helper.setUniformInt(   D_LIGHT_SHADOW_TEXTURES_SLOT, 
                                    shader_program->get_directional_shadow_map_location());

    std::vector<GLfloat> cascade_slots = main_light->get_cascaded_slots();
    for (size_t i = 0; i < NUM_CASCADES; i++) {

        glm::vec4 clip_end_slot = projection_matrix * glm::vec4(0.0f, 0.0f, -cascade_slots[i+1], 1.0f);
        uniform_helper.setUniformFloat(clip_end_slot.z, shader_program->get_cascade_endpoint_location(i));


        uniform_helper.setUniformMatrix4fv( cascade_light_matrices[i] * light_view,
                                            shader_program->get_directional_light_transform_location(i));
    }

    // GBUFFER
    gbuffer->read(  shader_program->get_g_position_location(),
                    shader_program->get_g_normal_location(),
                    shader_program->get_g_albedo_location(),
                    shader_program->get_uniform_material_id_location());

    std::vector<std::shared_ptr<PointLight>>& p_lights = scene->get_point_lights();
    shader_program->set_point_lights(p_lights, P_LIGHT_SHADOW_TEXTURES_SLOT);

    shader_program->set_noise_textures(WORLEY_NOISE_TEXTURES_SLOT);

    // CAMERA
    glm::vec3 camera_position = main_camera->get_camera_position();
    uniform_helper.setUniformVec3(camera_position, shader_program->get_eye_position_location());

    // MATERIALS
    std::vector<ObjMaterial> materials = scene->get_materials();
    for (size_t i = 0; i < materials.size(); i++) {

        materials[i].use_material(shader_program->get_uniform_material_locations(i));

    }

    // CLOUDS
    std::shared_ptr<Clouds> cloud = scene->get_clouds();
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

    uniform_helper.setUniformBlockBinding(  UNIFORM_LIGHT_MATRICES_BINDING,
                                            shader_program->get_light_matrics_id_location(),
                                            shader_program->get_id());

    uniform_helper.setUniformInt(RANDOM_NUMBERS_SLOT, shader_program->get_random_number_location());

    shader_program->validate_program();
}

void LightingPass::bind_buffers_for_lighting(   std::shared_ptr<GBuffer> gbuffer, 
                                                std::shared_ptr<Scene> scene,
                                                std::shared_ptr<Clouds> cloud)
{
    GLuint point_light_count = scene->get_point_light_count();
    std::shared_ptr<DirectionalLight> main_light = scene->get_sun();

    main_light->get_shadow_map()->read(D_LIGHT_SHADOW_TEXTURES_SLOT);

    cloud->read();

}

LightingPass::~LightingPass()
{

}