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

    void execute(   glm::mat4 projection,
                    std::shared_ptr<Camera> main_camera,
                    GLuint window_width, GLuint window_height,
                    std::shared_ptr<Scene> scene);

    void create_shader_program();

    void set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model);

    ~DirectionalShadowMapPass();

private:

    std::shared_ptr<ShadowMapShaderProgram> shader_program;

    DebugApp DebugApp_ins;
    UniformHelper uniform_helper;

};
