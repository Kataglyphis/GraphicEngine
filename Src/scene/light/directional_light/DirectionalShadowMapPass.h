#pragma once

#include "DirectionalLight.h"
#include "RenderPassSceneDependend.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "ViewFrustumCulling.h"

class DirectionalShadowMapPass : public RenderPassSceneDependend {
 public:
  DirectionalShadowMapPass();

  void execute(glm::mat4 projection, std::shared_ptr<Camera> main_camera,
               GLuint window_width, GLuint window_height,
               std::shared_ptr<Scene> scene);

  void create_shader_program();

  void set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model);

  ~DirectionalShadowMapPass();

 private:
  std::shared_ptr<ShaderProgram> shader_program;
};
