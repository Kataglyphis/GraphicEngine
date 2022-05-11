#include "GeometryPass.h"

GeometryPass::GeometryPass()
{

    this->uniform_helper = UniformHelper();
    create_shader_program();

    std::stringstream skybox_base_dir;
    skybox_base_dir << CMAKELISTS_DIR;
    skybox_base_dir << "/Resources/Textures/Skybox/DOOM2016/";

    stringstream texture_loading;
    std::array<std::string, 6> skybox_textures = {  "DOOM16RT.png",
                                                    "DOOM16LF.png",
                                                    "DOOM16UP.png",
                                                    "DOOM16DN.png",
                                                    "DOOM16FT.png",
                                                    "DOOM16BK.png"
    };

    std::vector<std::string> skybox_faces;

    for (int i = 0; i < skybox_textures.size(); i++) {

        texture_loading << skybox_base_dir.str() << skybox_textures[i];
        skybox_faces.push_back(texture_loading.str());
        texture_loading.str(std::string());

    }

    skybox = SkyBox(skybox_faces);

}

void GeometryPass::retrieve_geometry_pass_locations(glm::mat4 projection_matrix, 
                                                    glm::mat4 view_matrix,
                                                    std::vector<ObjMaterial>& materials, 
                                                    std::shared_ptr<Scene> scene)
{

    uniform_helper.setUniformMatrix4fv(projection_matrix, shader_program->get_projection_location());
    uniform_helper.setUniformMatrix4fv(view_matrix, shader_program->get_view_location());

    for (int i = 0; i < scene->get_texture_count(0); i++) {
        uniform_helper.setUniformInt(i, shader_program->get_uniform_texture_locations(i));
    }

    for (size_t i = 0; i < materials.size(); i++) {
        materials[i].use_material(shader_program->get_uniform_material_locations(i));
    }

    shader_program->validate_program();

    // Check if there any unchecked gl Errors from the render functions
    DebugApp_ins.areErrorPrintAll("From retrieve_geometry_pass_locations function in GeometryPass.");
}

void GeometryPass::execute( glm::mat4 projection_matrix, 
                            std::shared_ptr<Camera> main_camera, 
                            GLfloat window_width, GLfloat window_height,
                            GLuint gbuffer_id, GLfloat delta_time, 
                            std::shared_ptr<Scene> scene)
{

    glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_id);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, window_width, window_height);

    glm::mat4 view_matrix = main_camera->calculate_viewmatrix();

    shader_program->use_shader_program();

    std::vector<ObjMaterial> materials = scene->get_materials();

    retrieve_geometry_pass_locations(projection_matrix, view_matrix, materials, scene);

    scene->bind_textures_and_buffer();

    scene->render(this);

    skybox.draw_sky_box(projection_matrix, view_matrix, window_width, window_height, delta_time);

    // render the AABB for the clouds
    std::shared_ptr<Clouds> clouds = scene->get_clouds();
    clouds->render( projection_matrix,
                    view_matrix,
                    window_width, window_height);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Check if there any unchecked gl Errors from the render functions
    DebugApp_ins.areErrorPrintAll("From Execute function in GeometryPass.");

}

void GeometryPass::create_shader_program()
{
    this->shader_program = std::make_shared<GeometryPassShaderProgram>(GeometryPassShaderProgram{});
    this->shader_program->create_from_files("rasterizer/g_buffer_geometry_pass.vert",
                                            "rasterizer/g_buffer_geometry_pass.frag");
}

void GeometryPass::set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model)
{

    uniform_helper.setUniformMatrix4fv(model, shader_program->get_model_location());
    uniform_helper.setUniformMatrix4fv(normal_model, shader_program->get_normal_modal_location());

    // check if there any gl Errors
    DebugApp_ins.areErrorPrintAll("Error, from set_game_object_uniforms function in GeometryPass.");


}

GeometryPass::~GeometryPass()
{
}
