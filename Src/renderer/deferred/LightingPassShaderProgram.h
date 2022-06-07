#pragma once
#include "PointLight.h"
#include "ShaderProgram.h"
#include "host_device_shared.h"

class LightingPassShaderProgram : public ShaderProgram {
 public:
  LightingPassShaderProgram();

  ~LightingPassShaderProgram();

 private:
};
