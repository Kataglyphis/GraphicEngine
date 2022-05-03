#include "DirectionalShadowMapPass.h"

DirectionalShadowMapPass::DirectionalShadowMapPass()
{
}

DirectionalShadowMapPass::DirectionalShadowMapPass(std::shared_ptr<ShadowMapShaderProgram> shader_program)
{
    this->shader_program = shader_program;
}

void DirectionalShadowMapPass::execute(std::shared_ptr<DirectionalLight> d_light, glm::mat4 view_matrix,
                                       bool first_person_mode, Scene* scene)
{
    
    shader_program->use_shader_program();

    glViewport(0, 0, d_light->get_shadow_map()->get_shadow_width(), d_light->get_shadow_map()->get_shadow_height());

    d_light->get_shadow_map()->write();
    d_light->get_shadow_map()->write_light_matrices(d_light->get_cascaded_light_matrices());
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT); // peter panning

    scene->render(this, first_person_mode);
    shader_program->validate_program();

    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalShadowMapPass::set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model)
{
    // DO NOT set neither normal model nor material_id hence we didn't need it
    glUniformMatrix4fv(shader_program->get_model_location(), 1, GL_FALSE, glm::value_ptr(model));
}

bool DirectionalShadowMapPass::use_terrain_textures()
{
    return false;
}

DirectionalShadowMapPass::~DirectionalShadowMapPass()
{
}