#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ComputeShaderProgram.h"

#include "texture_unit_slots.h"
#include "DebugApp.h"

#include <random>
#include <memory>

class Noise
{
public:

	Noise();

	void create_worley_noise();
	void create_grad_noise();

	void read_worley_noise(GLenum start_buffer_index);
	void read_grad_noise(GLenum start_buffer_index);

	void update();

	void set_num_cells(GLuint num_cells_per_axis, GLuint index);

	~Noise();

private:

	void generate_cells(GLuint num_cells_per_axis, GLuint cell_index);
	void generate_textures();
	void delete_textures();

	GLuint texture_1_id;
	GLuint texture_dim_1;
	ComputeShaderProgram texture_1_shader_program;

	GLuint cell_ids[NUM_CELLS];
	GLuint num_cells_per_axis[NUM_CELLS];

	GLuint texture_2_id;
	GLuint texture_dim_2;
	ComputeShaderProgram texture_2_shader_program;

	std::shared_ptr<GLfloat[]> cell_data [NUM_CELLS];

	DebugApp DebugApp_ins;

};

