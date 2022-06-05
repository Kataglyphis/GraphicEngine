#pragma once
#include "ShaderProgram.h"
#include "host_device_shared.h"

#include <glad/glad.h>

class ComputeShaderProgram : public ShaderProgram {
  public:
  ComputeShaderProgram();

  void reload();

  ~ComputeShaderProgram();

  private:
};
