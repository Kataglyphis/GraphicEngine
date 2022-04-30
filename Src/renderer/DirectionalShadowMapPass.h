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
    DirectionalShadowMapPass(std::shared_ptr<ShadowMapShaderProgram> shader_program);

    void execute(std::shared_ptr<DirectionalLight> d_light, glm::mat4 viewmatrix,
                          bool first_person_mode, Scene* scene);

    void set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model);

    bool use_terrain_textures();

    ~DirectionalShadowMapPass();

private:

    std::shared_ptr<ShadowMapShaderProgram> shader_program;

    glErrorChecker glErrorChecker_ins;
};

