#pragma once

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#include "DirectionalShadowMapPass.hpp"
#include "GBuffer.hpp"
#include "GeometryPass.hpp"
#include "LightingPass.hpp"
#include "OmniShadowMapPass.hpp"
#include "ShaderIncludes.hpp"

class Renderer {
 public:
  Renderer(GLuint window_width, GLuint window_height);

  void drawFrame(std::shared_ptr<Camera> main_camera,
                 std::shared_ptr<Scene> scene, glm::mat4 projection_matrix,
                 GLfloat delta_time);

  void update_window_params(GLuint window_width, GLuint window_height);
  void reload_shader_programs();

  ~Renderer();

 private:
  GLuint window_width, window_height;

  std::shared_ptr<GBuffer> gbuffer;

  ShaderIncludes shader_includes;

  // all render passes
  std::vector<std::shared_ptr<RenderPass>> render_passes;

  std::shared_ptr<OmniShadowMapPass> omni_shadow_map_pass;
  std::shared_ptr<DirectionalShadowMapPass> directional_shadow_map_pass;
  std::shared_ptr<GeometryPass> geometry_pass;
  std::shared_ptr<LightingPass> lighting_pass;
};
