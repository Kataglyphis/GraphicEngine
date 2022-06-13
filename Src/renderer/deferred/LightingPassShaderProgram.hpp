#pragma once
#include "PointLight.hpp"
#include "ShaderProgram.hpp"
#include "host_device_shared.hpp"

class LightingPassShaderProgram : public ShaderProgram {
 public:
  LightingPassShaderProgram();

  ~LightingPassShaderProgram();

 private:
};
