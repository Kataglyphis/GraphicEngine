#include "DirectionalShadowMapPass.h"

DirectionalShadowMapPass::DirectionalShadowMapPass()
{
}

DirectionalShadowMapPass::DirectionalShadowMapPass(std::shared_ptr<ShadowMapShaderProgram> shader_program)
{
    this->shader_program = shader_program;
}

void DirectionalShadowMapPass::execute(std::shared_ptr<DirectionalLight> d_light, glm::mat4 view_matrix,
                                                                    bool first_person_mode, std::shared_ptr<Scene> scene)
{
    
    shader_program->use_shader_program();

    glViewport(0, 0, d_light->get_shadow_map()->get_shadow_width(), d_light->get_shadow_map()->get_shadow_height());

    //glm::mat4 l_traf[NUM_CASCADES];

    for (unsigned int i = 0; i < d_light->get_shadow_map()->get_num_active_cascades(); i++) {

        d_light->get_shadow_map()->write(i);
        glClear(GL_DEPTH_BUFFER_BIT);

        std::vector<glm::mat4> cascade_light_matrices = d_light->get_cascaded_light_matrices();
        glm::mat4 l_traf = cascade_light_matrices[i] * d_light->get_light_view_matrix();
        shader_program->set_directional_light_transform(l_traf);

        shader_program->validate_program();

        scene->render(this, first_person_mode);

    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalShadowMapPass::set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model, GLuint material_id)
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