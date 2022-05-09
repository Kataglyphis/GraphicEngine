#pragma once

#include "DirectionalShadowMapPass.h"
#include "OmniShadowMapPass.h"
#include "LightingPass.h"
#include "GeometryPass.h"
#include "GBuffer.h"

class Renderer
{
public:

	Renderer(GLuint window_width, GLuint window_height);

	void drawFrame( std::shared_ptr<Camera> main_camera,
                    std::shared_ptr<Scene> scene,
                    glm::mat4 projection_matrix,
                    GLfloat delta_time);

    void update_window_params(GLuint window_width, GLuint window_height);
    void reload_shader_programs();

	~Renderer();

private:

    GLuint window_width, window_height;

    std::shared_ptr<GBuffer> gbuffer;

    // all render passes
    OmniShadowMapPass           omni_shadow_map_pass;
    DirectionalShadowMapPass    directional_shadow_map_pass;
    GeometryPass                geometry_pass;
    LightingPass                lighting_pass;

};

