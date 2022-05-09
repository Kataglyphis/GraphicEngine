#pragma once
#include "RenderPassSceneDependend.h"
#include "PointLight.h"
#include "OmniDirShadowShaderProgram.h"
#include "ViewFrustumCulling.h"
#include "Clouds.h"
#include "Scene.h"

#include "DebugApp.h"

class OmniShadowMapPass :
    public RenderPassSceneDependend
{
public:

    OmniShadowMapPass();

    void set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model);

    void execute(   std::shared_ptr<PointLight> p_light,
                    std::shared_ptr<Scene> scene);

    void create_shader_program();

    ~OmniShadowMapPass();

private:

    std::shared_ptr<OmniDirShadowShaderProgram> shader_program;

    // this is for GL error checking
    DebugApp DebugApp_ins;
};

