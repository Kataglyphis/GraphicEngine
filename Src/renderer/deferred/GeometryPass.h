#pragma once
#include "RenderPassSceneDependend.h"
#include "DirectionalLight.h"
#include "GeometryPassShaderProgram.h"
#include "SkyBox.h"
#include "ViewFrustumCulling.h"
#include "Camera.h"
#include "Clouds.h"
#include "Scene.h"
#include "SkyBox.h"

#include <array>

class GeometryPass : public RenderPassSceneDependend {
  public:
  GeometryPass();

  void execute(glm::mat4 projection_matrix, std::shared_ptr<Camera> main_camera, GLuint window_width, GLuint window_height, GLuint gbuffer_id,
    GLfloat delta_time, std::shared_ptr<Scene>);

  void create_shader_program();

  void set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model);

  ~GeometryPass();

  private:
  std::shared_ptr<GeometryPassShaderProgram> shader_program;

  SkyBox skybox;
};
