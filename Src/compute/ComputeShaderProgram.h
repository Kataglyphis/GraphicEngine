#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.h"
#include "host_device_shared.h"

class ComputeShaderProgram : public ShaderProgram {
 public:
  ComputeShaderProgram();

  void reload();

  ~ComputeShaderProgram();

 private:
};
