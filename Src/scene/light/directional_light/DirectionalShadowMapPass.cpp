#include "DirectionalShadowMapPass.h"
#include <memory>

DirectionalShadowMapPass::DirectionalShadowMapPass()
{
    create_shader_program();
    this->uniform_helper = UniformHelper();
}

void DirectionalShadowMapPass::execute( glm::mat4 projection, 
                                        std::shared_ptr<Camera> main_camera,
                                        GLuint window_width, GLuint window_height,
                                        std::shared_ptr<Scene> scene)
{

    std::shared_ptr<DirectionalLight> sun = scene->get_sun();
    //retreive shadow map before our geometry pass
    sun->calc_orthogonal_projections(   main_camera->calculate_viewmatrix(),
                                        main_camera->get_fov(), window_width, window_height,
                                        NUM_CASCADES);

    shader_program->use_shader_program();

    sun->get_shadow_map()->write();

    glViewport(0, 0,    sun->get_shadow_map()->get_shadow_width(),
                        sun->get_shadow_map()->get_shadow_height());

    glClear(GL_DEPTH_BUFFER_BIT);
    //glCullFace(GL_FRONT); // avoid peter panning
    sun->get_shadow_map()->write_light_matrices(sun->get_cascaded_light_matrices());

    uniform_helper.setUniformBlockBinding(  UNIFORM_LIGHT_MATRICES_BINDING,
                                            shader_program->get_light_matrics_id_location(),
                                            shader_program->get_id());

    glm::mat4 view_matrix = main_camera->calculate_viewmatrix();

    uniform_helper.setUniformMatrix4fv(projection * view_matrix, shader_program->get_VP_location());

    scene->render(this);

    //glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    shader_program->validate_program();
}

void DirectionalShadowMapPass::create_shader_program()
{
    shader_program = std::make_shared<ShadowMapShaderProgram>(ShadowMapShaderProgram{});
    shader_program->create_from_files(  "rasterizer/shadows/directional_shadow_map.vert",
                                        "rasterizer/shadows/directional_shadow_map.geom",
                                        "rasterizer/shadows/directional_shadow_map.frag");
}

void DirectionalShadowMapPass::set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model)
{
    // DO NOT set neither normal model nor material_id hence we didn't need it
    glUniformMatrix4fv(shader_program->get_model_location(), 1, GL_FALSE, glm::value_ptr(model));

}

DirectionalShadowMapPass::~DirectionalShadowMapPass()
{
}