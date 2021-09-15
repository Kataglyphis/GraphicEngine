#include "GeometryPass.h"



GeometryPass::GeometryPass()
{
}

GeometryPass::GeometryPass(GeometryPassShaderProgram* shader_program)
{

    this->shader_program = shader_program;

    // create skybox
    std::vector<std::string> skybox_faces;
    skybox_faces.push_back("Textures/Skybox/space_left.jpg");
    skybox_faces.push_back("Textures/Skybox/space_right.jpg");
    skybox_faces.push_back("Textures/Skybox/space_up.jpg");
    skybox_faces.push_back("Textures/Skybox/space_down.jpg");
    skybox_faces.push_back("Textures/Skybox/space_front.jpg");
    skybox_faces.push_back("Textures/Skybox/space_back.jpg");

    skybox = SkyBox(skybox_faces);

}

void GeometryPass::retrieve_geometry_pass_locations(glm::mat4 projection_matrix, glm::mat4 view_matrix , Terrain_Generator* terrain_generator)
{

    glUniformMatrix4fv(shader_program->get_projection_location(), 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(shader_program->get_view_location(), 1, GL_FALSE, glm::value_ptr(view_matrix));

    terrain_generator->retreive_uniform_locations(shader_program);

    shader_program->validate_program();

    // Check if there any unchecked gl Errors from the render functions
    glErrorChecker.areErrorPrintAll("From retrieve_geometry_pass_locations function in GeometryPass.");
}

void GeometryPass::execute(glm::mat4 projection_matrix, glm::mat4 view_matrix, GLfloat window_width, GLfloat window_height,
                                                GLuint gbuffer_id, bool first_person_mode, GLfloat delta_time, Scene* scene)
{

    glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_id);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, window_width, window_height);

    skybox.draw_sky_box(projection_matrix, view_matrix, window_width, window_height, delta_time);

    shader_program->use_shader_program();

    retrieve_geometry_pass_locations(projection_matrix, view_matrix, scene->get_terrain_generator());

    scene->render(this, first_person_mode);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Check if there any unchecked gl Errors from the render functions
    glErrorChecker.areErrorPrintAll("From Execute function in GeometryPass.");

}

void GeometryPass::set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model, GLuint material_id)
{
    glUniformMatrix4fv(shader_program->get_model_location(), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader_program->get_normal_modal_location(), 1, GL_FALSE, glm::value_ptr(normal_model));

    glUniform1i(shader_program->get_material_id_location(), material_id);

    // check if there any gl Errors
    glErrorChecker.areErrorPrintAll("Error, from set_game_object_uniforms function in GeometryPass.");

    //bool useTexture = true;
    //terrain_generator->render_plants(model_matrix_terrain, is_chunk_rendered_flags, useTexture, shader_program->get_model_location());


}

bool GeometryPass::use_terrain_textures()
{
    return true;
}

GeometryPass::~GeometryPass()
{
}
