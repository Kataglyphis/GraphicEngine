#include "ShadowMapShaderProgram.h"

ShadowMapShaderProgram::ShadowMapShaderProgram()
{
}

GLuint ShadowMapShaderProgram::get_model_location()
{
	return uniform_model_location;
}

GLuint ShadowMapShaderProgram::get_directional_light_transform_location()
{
	return uniform_directional_light_transform_location;
}

void ShadowMapShaderProgram::set_directional_light_transform(glm::mat4& light_transform)
{
	glUniformMatrix4fv(uniform_directional_light_transform_location, 1, GL_FALSE, glm::value_ptr(light_transform));
}

ShadowMapShaderProgram::~ShadowMapShaderProgram()
{
}

void ShadowMapShaderProgram::retrieve_uniform_locations()
{
	uniform_model_location = glGetUniformLocation(program_id, "model");
	uniform_directional_light_transform_location = glGetUniformLocation(program_id, "directional_light_transform");
}
