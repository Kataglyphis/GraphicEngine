#include "LightingPass.h"

LightingPass::LightingPass()
{
}

void LightingPass::init()
{

    current_offset          = glm::vec3(0.0f);

    create_shader_program();

}

void LightingPass::execute( glm::mat4 projection_matrix, 
                            std::shared_ptr<Camera> main_camera,
                            std::shared_ptr<Scene> scene,
                            std::shared_ptr<GBuffer> gbuffer,
                            float delta_time)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::shared_ptr<DirectionalLight>           main_light = scene->get_sun();
    std::shared_ptr<Clouds>                     cloud = scene->get_clouds();
    std::vector<std::shared_ptr<PointLight>>    point_lights = scene->get_point_lights();

    shader_program->use_shader_program();

    set_uniforms (  projection_matrix, 
                    main_camera, 
                    scene,
                    gbuffer,
                    delta_time);

    //bind textures to their units
    main_light->get_shadow_map()->read(D_LIGHT_SHADOW_TEXTURES_SLOT);
    cloud->read();

    for (int i = 0; i < static_cast<GLuint>(point_lights.size()); i++) {
        point_lights[i]->get_omni_shadow_map()->read(P_LIGHT_SHADOW_TEXTURES_SLOT + i);
    }

    // render screen filling quad
    quad.render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightingPass::create_shader_program()
{
    shader_program = std::make_shared<LightingPassShaderProgram>(LightingPassShaderProgram{});
    shader_program->create_from_files(  "rasterizer/g_buffer_lighting_pass.vert",
                                        "rasterizer/g_buffer_lighting_pass.frag");
}

void LightingPass::set_uniforms(    glm::mat4 projection_matrix, 
                                    std::shared_ptr<Camera> main_camera,
                                    std::shared_ptr<Scene> scene,
                                    std::shared_ptr<GBuffer> gbuffer,
                                    float delta_time)
{

    // VP
    glm::mat4 view_matrix = main_camera->calculate_viewmatrix();
    shader_program->setUniformMatrix4fv(view_matrix, "view");
    shader_program->setUniformMatrix4fv(projection_matrix, "projection");

    // SUN UNIFORMS
    std::shared_ptr<DirectionalLight>   main_light = scene->get_sun();
    shader_program->setUniformFloat(    main_light->get_radiance(), "directional_light.base.radiance");
    shader_program->setUniformVec3(     main_light->get_color(), "directional_light.base.color");
    shader_program->setUniformVec3(     main_light->get_direction(), "directional_light.direction");

    // EVERYTHING REGARDING THE SHADOW CASCADE
    glm::mat4 light_view = main_light->get_light_view_matrix();
    shader_program->setUniformInt(  D_LIGHT_SHADOW_TEXTURES_SLOT, 
                                    "directional_shadow_maps");

    std::vector<glm::mat4> cascade_light_matrices   = main_light->get_cascaded_light_matrices();
    std::vector<GLfloat> cascade_slots              = main_light->get_cascaded_slots();

    for (size_t i = 0; i < NUM_CASCADES; i++) {

        glm::vec4 clip_end_slot = projection_matrix * glm::vec4(0.0f, 0.0f, -cascade_slots[i+1], 1.0f);
        char loc_buff[100] = { '\0' };
        snprintf(loc_buff, sizeof(loc_buff), "cascade_endpoints[%zd]", i);
        shader_program->setUniformFloat(clip_end_slot.z, loc_buff);

    }

    shader_program->setUniformInt(main_light->get_shadow_map()->get_pcf_radius(), "pcf_radius");

    // READ GBUFFER
    gbuffer->read(shader_program);

    // POINT LIGHTS
    std::vector<std::shared_ptr<PointLight>>    point_lights = scene->get_point_lights();
    GLuint                                      point_light_count = scene->get_point_light_count();


    shader_program->setUniformInt(static_cast<uint32_t>(point_lights.size()), "point_light_count");

    for (size_t i = 0; i < static_cast<uint32_t>(point_lights.size()); i++) {

        char loc_buff[100] = { '\0' };

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].base.color", i);
        shader_program->setUniformVec3(point_lights[i]->get_color(), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].base.radiance", i);
        shader_program->setUniformFloat(point_lights[i]->get_radiance(), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].position", i);
        shader_program->setUniformVec3(point_lights[i]->get_position(), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].constant", i);
        shader_program->setUniformFloat(point_lights[i]->get_constant_factor(), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].linear", i);
        shader_program->setUniformFloat(point_lights[i]->get_linear_factor(), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].exponent", i);
        shader_program->setUniformFloat(point_lights[i]->get_exponent_factor(), loc_buff);

        //unofrom locations for omni shadow maps
        snprintf(loc_buff, sizeof(loc_buff), "omni_shadow_maps[%zd].shadow_map", i);
        shader_program->setUniformInt((GLint)(P_LIGHT_SHADOW_TEXTURES_SLOT + i), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "omni_shadow_maps[%zd].far_plane", i);
        shader_program->setUniformInt(point_lights[i]->get_far_plane(), loc_buff);

    }

    // CAMERA
    glm::vec3 camera_position = main_camera->get_camera_position();
    shader_program->setUniformVec3(camera_position, "eye_position");

    // MATERIALS
    std::vector<ObjMaterial> materials = scene->get_materials();
    for (size_t i = 0; i < materials.size(); i++) {

        char loc_buff[100] = { '\0' };

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].ambient", i);
        shader_program->setUniformVec3(materials[i].get_ambient(), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].diffuse", i);
        shader_program->setUniformVec3(materials[i].get_diffuse(), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].specular", i);
        shader_program->setUniformVec3(materials[i].get_specular(), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].transmittance", i);
        shader_program->setUniformVec3(materials[i].get_transmittance(), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].emission", i);
        shader_program->setUniformVec3(materials[i].get_emission(), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].shininess", i);
        shader_program->setUniformFloat(materials[i].get_shininess(), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].ior", i);
        shader_program->setUniformFloat(materials[i].get_ior(), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].dissolve", i);
        shader_program->setUniformFloat(materials[i].get_dissolve(), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].illum", i);
        shader_program->setUniformInt(materials[i].get_illum(), loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].textureID", i);
        shader_program->setUniformInt(materials[i].get_textureID(), loc_buff);

    }

    // CLOUDS
    
    std::shared_ptr<Clouds> cloud = scene->get_clouds();

    shader_program->setUniformVec3(cloud->get_rad(), "cloud.rad");
    GLfloat velocity = cloud->get_movement_speed() * delta_time;
    current_offset = current_offset + cloud->get_movement_direction() * velocity;
    shader_program->setUniformVec3(current_offset, "cloud.offset");
    shader_program->setUniformMatrix4fv(cloud->get_model(), "cloud.model_to_world");
    shader_program->setUniformFloat(cloud->get_scale(), "cloud.scale");
    shader_program->setUniformFloat(1.f - cloud->get_density(), "cloud.threshold");
    shader_program->setUniformFloat(cloud->get_pillowness(), "cloud.pillowness");
    shader_program->setUniformFloat(cloud->get_cirrus_effect(), "cloud.cirrus_effect");

    if (cloud->get_powder_effect()) {
        shader_program->setUniformInt(true, "cloud.powder_effect");
    }
    else {
        shader_program->setUniformInt(false, "cloud.powder_effect");
    } 

    shader_program->setUniformBlockBinding( UNIFORM_LIGHT_MATRICES_BINDING,
                                            "LightSpaceMatrices");

    shader_program->setUniformInt(RANDOM_NUMBERS_SLOT, "random_number");

    shader_program->validate_program();
}

LightingPass::~LightingPass()
{

}