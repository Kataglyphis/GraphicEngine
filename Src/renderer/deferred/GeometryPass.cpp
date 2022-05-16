#include "GeometryPass.h"

GeometryPass::GeometryPass() :
                    skybox()
{

    create_shader_program();

}

void GeometryPass::execute( glm::mat4               projection_matrix, 
                            std::shared_ptr<Camera> main_camera, 
                            GLfloat                 window_width, 
                            GLfloat                 window_height,
                            GLuint                  gbuffer_id, 
                            GLfloat                 delta_time, 
                            std::shared_ptr<Scene>  scene)
{

    glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_id);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, window_width, window_height);

    shader_program->use_shader_program();

    glm::mat4 view_matrix               = main_camera->calculate_viewmatrix();
    std::vector<ObjMaterial> materials  = scene->get_materials();

    shader_program->setUniformMatrix4fv(projection_matrix, "projection");
    shader_program->setUniformMatrix4fv(view_matrix, "view");

    for (int i = 0; i < scene->get_texture_count(0); i++) {

        char loc_buff[100] = { '\0' };
        snprintf(loc_buff, sizeof(loc_buff), "model_textures[%zd]", i);
        shader_program->setUniformInt(MODEL_TEXTURES_SLOT + i, loc_buff);

    }

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

    shader_program->validate_program();

    scene->bind_textures_and_buffer();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //aabb->render();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // 

    std::vector<std::shared_ptr<GameObject>> game_objects = 
                                            scene->get_game_objects();

    for (std::shared_ptr<GameObject> object : game_objects) {

        /* if (object_is_visible(object)) {*/

        set_game_object_uniforms(   object->get_world_trafo(),
                                    object->get_normal_world_trafo());

        object->render();
        //}

    }

    skybox.draw_sky_box(projection_matrix, view_matrix, window_width, window_height, delta_time);

    // render the AABB for the clouds
    std::shared_ptr<Clouds> clouds = scene->get_clouds();
    clouds->render( projection_matrix,
                    view_matrix,
                    window_width, window_height);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GeometryPass::create_shader_program()
{
    this->shader_program = std::make_shared<GeometryPassShaderProgram>(GeometryPassShaderProgram{});
    this->shader_program->create_from_files("rasterizer/g_buffer_geometry_pass.vert",
                                            "rasterizer/g_buffer_geometry_pass.frag");
}

void GeometryPass::set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model)
{

    shader_program->setUniformMatrix4fv(model, "model");
    shader_program->setUniformMatrix4fv(normal_model, "normal_model");

}

GeometryPass::~GeometryPass()
{
}
