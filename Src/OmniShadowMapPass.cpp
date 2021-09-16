#include "OmniShadowMapPass.h"

OmniShadowMapPass::OmniShadowMapPass()
{
}

OmniShadowMapPass::OmniShadowMapPass(OmniDirShadowShaderProgram* shader_program)
{
    this->shader_program = shader_program;
}

void OmniShadowMapPass::set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model, GLuint material_id)
{
    // DO NOT set neither normal model nor material_id hence we didn't need it
    glUniformMatrix4fv(shader_program->get_model_location(), 1, GL_FALSE, glm::value_ptr(model));
}

bool OmniShadowMapPass::use_terrain_textures()
{
    return false;
}

void OmniShadowMapPass::execute(PointLight* p_light, bool first_person_mode, Scene* scene)
{
    shader_program->use_shader_program();

    glViewport(0, 0, p_light->get_omni_shadow_map()->get_shadow_width(), p_light->get_omni_shadow_map()->get_shadow_height());

    p_light->get_omni_shadow_map()->write();
    glClear(GL_DEPTH_BUFFER_BIT);

    //uniform_omni_shadow_model_location = omni_dir_shadow_shader_program.get_model_location();

    glUniform3f(shader_program->get_omni_light_pos_location(), p_light->get_position().x, p_light->get_position().y, p_light->get_position().z);
    glUniform1f(shader_program->get_far_plane_location(), p_light->get_far_plane());
    shader_program->set_light_matrices(p_light->calculate_light_transform());

    shader_program->validate_program();

    scene->render(this, first_person_mode);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glErrorChecker_ins.areErrorPrintAll("From execute function OmniShadowMapPass");


}

OmniShadowMapPass::~OmniShadowMapPass()
{
}
