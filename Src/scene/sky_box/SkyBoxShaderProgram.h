#pragma once
#include "ShaderProgram.h"

class SkyBoxShaderProgram:
	public ShaderProgram
{
public:

	SkyBoxShaderProgram();

	GLuint get_projection_location();
	GLuint get_view_location();
	GLuint get_uniform_skyBoxMaterialID_location();
	GLuint get_uniform_samplerCube_location();

	~SkyBoxShaderProgram();

private:

	GLuint	uniform_view_location, uniform_projection_location,
			uniform_skyBoxMaterialID_location, 
			uniform_samplerCube_location;

	void retrieve_uniform_locations();
};

