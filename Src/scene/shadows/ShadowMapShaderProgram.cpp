#include "ShadowMapShaderProgram.h"

ShadowMapShaderProgram::ShadowMapShaderProgram()
{
}

GLuint ShadowMapShaderProgram::get_model_location()
{
	return uniform_model_location;
}

GLuint ShadowMapShaderProgram::get_light_matrics_id_location()
{
	return uniform_light_matrics_id;
}

GLuint ShadowMapShaderProgram::get_VP_location()
{
	return uniform_VP;
}

ShadowMapShaderProgram::~ShadowMapShaderProgram()
{
}

void ShadowMapShaderProgram::retrieve_uniform_locations()
{
	uniform_model_location		= glGetUniformLocation(program_id, "model");
	uniform_light_matrics_id	= glGetUniformBlockIndex(program_id, "LightSpaceMatrices");

	uniform_VP					= glGetUniformLocation(program_id, "VP");

}
