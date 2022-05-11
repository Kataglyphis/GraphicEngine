#include "ComputeShaderProgram.h"

ComputeShaderProgram::ComputeShaderProgram()
{
}

GLuint ComputeShaderProgram::get_cell_location(GLuint index)
{
	return uniform_cell_locations[index];
}

GLuint ComputeShaderProgram::get_num_cell_location(GLuint index)
{
	return uniform_num_cell_locations[index];
}

GLuint ComputeShaderProgram::get_noise_image_location()
{
	return uniform_noise_image_location;
}

void ComputeShaderProgram::retrieve_uniform_locations()
{
	uniform_noise_image_location = glGetUniformLocation(program_id, "noise");

	for (size_t i = 0; i < NUM_CELL_POSITIONS; i++) {

		char loc_buff[100] = { '\0' };
		snprintf(loc_buff, sizeof(loc_buff), "cell_positions[%zd]", i);
		uniform_cell_locations[i] = glGetUniformLocation(program_id, loc_buff);

		snprintf(loc_buff, sizeof(loc_buff), "num_cells[%zd]", i);
		uniform_num_cell_locations[i] = glGetUniformLocation(program_id, loc_buff);

	}

}

void ComputeShaderProgram::reload()
{
	create_computer_shader_program_from_file(compute_location);
}

ComputeShaderProgram::~ComputeShaderProgram()
{
}
