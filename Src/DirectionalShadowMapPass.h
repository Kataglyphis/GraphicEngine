#pragma once
#include "RenderPassSceneDependend.h"
#include "DirectionalLight.h"
#include "ShadowMapShaderProgram.h"
#include "ViewFrustumCulling.h"
#include "Scene.h"

#include "glErrorChecker.h"

class DirectionalShadowMapPass :
    public RenderPassSceneDependend
{
public:
    DirectionalShadowMapPass();
    DirectionalShadowMapPass(ShadowMapShaderProgram* shader_program);

    void execute(DirectionalLight* d_light, glm::mat4 viewmatrix,
                          bool first_person_mode, Scene* scene);

    void set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model, GLuint material_id);

    bool use_terrain_textures();

    ~DirectionalShadowMapPass();

private:

    ShadowMapShaderProgram* shader_program;

    glErrorChecker glErrorChecker_ins;
};

