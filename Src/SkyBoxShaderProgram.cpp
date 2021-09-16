#include "SkyBoxShaderProgram.h"

SkyBoxShaderProgram::SkyBoxShaderProgram()
{
}

GLuint SkyBoxShaderProgram::get_projection_location()
{
	return uniform_projection_location;
}

GLuint SkyBoxShaderProgram::get_view_location()
{
	return uniform_view_location;
}

SkyBoxShaderProgram::~SkyBoxShaderProgram()
{
}

void SkyBoxShaderProgram::retrieve_uniform_locations()
{

	uniform_projection_location = glGetUniformLocation(program_id, "projection");
	uniform_view_location = glGetUniformLocation(program_id, "view");

}
