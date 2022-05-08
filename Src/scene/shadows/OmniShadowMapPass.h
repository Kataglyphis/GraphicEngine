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
    OmniShadowMapPass(std::shared_ptr<OmniDirShadowShaderProgram> shader_program);

    void set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model);

    bool use_terrain_textures();

    void execute(   std::shared_ptr<PointLight> p_light,
                    std::shared_ptr<Scene> scene);

    ~OmniShadowMapPass();

private:

    std::shared_ptr<OmniDirShadowShaderProgram> shader_program;

    // this is for GL error checking
    DebugApp DebugApp_ins;
};

