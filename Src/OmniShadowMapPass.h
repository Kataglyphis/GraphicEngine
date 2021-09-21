#pragma once
#include "RenderPassSceneDependend.h"
#include "PointLight.h"
#include "OmniDirShadowShaderProgram.h"
#include "ViewFrustumCulling.h"
#include "Clouds.h"
#include "Scene.h"

#include "glErrorChecker.h"

class OmniShadowMapPass :
    public RenderPassSceneDependend
{
public:

    OmniShadowMapPass();
    OmniShadowMapPass(std::shared_ptr<OmniDirShadowShaderProgram> shader_program);

    void set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model, GLuint material_id);

    bool use_terrain_textures();

    void execute(PointLight* p_light, bool first_person_mode, Scene* scene);

    ~OmniShadowMapPass();

private:

    std::shared_ptr<OmniDirShadowShaderProgram> shader_program;

    // this is for GL error checking
    glErrorChecker glErrorChecker_ins;
};

