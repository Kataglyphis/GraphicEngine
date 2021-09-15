#include "OmniDirShadowShaderProgram.h"

OmniDirShadowShaderProgram::OmniDirShadowShaderProgram()
{
}

GLuint OmniDirShadowShaderProgram::get_model_location()
{
	return uniform_model_location;
}

GLuint OmniDirShadowShaderProgram::get_omni_light_pos_location()
{
	return uniform_omni_light_pos_location;
}

GLuint OmniDirShadowShaderProgram::get_far_plane_location()
{
	return uniform_far_plane_location;
}

void OmniDirShadowShaderProgram::reload()
{
	create_from_files(this->vertex_location, this->geometry_location, this->fragment_location);
}

void OmniDirShadowShaderProgram::set_light_matrices(std::vector<glm::mat4> light_matrices)
{
	for (size_t i = 0; i < 6; i++) {
		glUniformMatrix4fv(uniform_light_matrices_locations[i], 1, GL_FALSE, glm::value_ptr(light_matrices[i]));
	}
}

void OmniDirShadowShaderProgram::retrieve_uniform_locations()
{

	uniform_model_location = glGetUniformLocation(program_id, "model");;
	uniform_omni_light_pos_location = glGetUniformLocation(program_id, "light_pos");;
	uniform_far_plane_location = glGetUniformLocation(program_id, "far_plane");

	for (size_t i = 0; i < 6; i++) {

		char loc_buff[100] = { '\0' };
		snprintf(loc_buff, sizeof(loc_buff), "light_matrices[%zd]", i);
		uniform_light_matrices_locations[i] = glGetUniformLocation(program_id, loc_buff);

	}

}

OmniDirShadowShaderProgram::~OmniDirShadowShaderProgram()
{
}