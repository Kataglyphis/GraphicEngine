#pragma once
#include "RenderPassSceneDependend.h"
#include "DirectionalLight.h"
#include "ShadowMapShaderProgram.h"
#include "ViewFrustumCulling.h"
#include "Scene.h"
#include "UniformHelper.h"
#include "DebugApp.h"

class DirectionalShadowMapPass :
    public RenderPassSceneDependend
{
public:
    DirectionalShadowMapPass();
    DirectionalShadowMapPass(std::shared_ptr<ShadowMapShaderProgram> shader_program);

    void execute(   std::shared_ptr<DirectionalLight> d_light, 
                    glm::mat4 viewmatrix,
                    glm::mat4 projection,
                    bool first_person_mode, Scene* scene);

    void set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model);

    ~DirectionalShadowMapPass();

private:

    std::shared_ptr<ShadowMapShaderProgram> shader_program;

    DebugApp DebugApp_ins;
    UniformHelper uniform_helper;

};

