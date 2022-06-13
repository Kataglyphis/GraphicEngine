#pragma once
#include "ShaderProgram.hpp"
#include "host_device_shared.hpp"

class GeometryPassShaderProgram : public ShaderProgram {
 public:
  GeometryPassShaderProgram();

  GLuint get_program_id() { return program_id; }

  ~GeometryPassShaderProgram();

 protected:
};
