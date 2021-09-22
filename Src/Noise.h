#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ComputeShaderProgram.h"

#include "glErrorChecker.h"

#include <random>

class Noise
{
public:

	Noise();

	void create_worley_noise();
	void create_grad_noise();

	void read_worley_noise(GLenum start_buffer_index);
	void read_grad_noise(GLenum start_buffer_index);

	void init();
	void update();

	void set_num_cells(GLuint num_cells_per_axis, GLuint index);

	~Noise();

private:

	void generate_cells(GLuint num_cells_per_axis, GLuint cell_index);
	void generate_textures();
	void delete_textures();

	GLuint texture_1, texture_2;
	GLuint cell_ids[NUM_CELLS];

	GLuint texture_dim_1, texture_dim_2;
	GLuint num_cells_per_axis[NUM_CELLS];

	ComputeShaderProgram texture_1_shader_program;
	ComputeShaderProgram texture_2_shader_program;

	GLfloat* cell_data [NUM_CELLS];

	glErrorChecker glErrorChecker_ins;

};

