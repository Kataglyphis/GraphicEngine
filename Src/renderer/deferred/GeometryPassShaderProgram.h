#pragma once
#include "ShaderProgram.h"
#include "host_device_shared.h"

class GeometryPassShaderProgram:
	public ShaderProgram
{

public:

	GeometryPassShaderProgram();

	GLuint get_program_id() { return program_id; }

	~GeometryPassShaderProgram();

protected:

};

