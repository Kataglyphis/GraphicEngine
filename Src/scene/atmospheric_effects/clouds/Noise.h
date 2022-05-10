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


// inspired by: http://advances.realtimerendering.com/s2015/The%20Real-time%20Volumetric%20Cloudscapes%20of%20Horizon%20-%20Zero%20Dawn%20-%20ARTR.pdf
class Noise
{
public:

	Noise();

	void create_res128_noise();
	void create_res32_noise();

	void read_res128_noise();
	void read_res32_noise();

	void update();

	void set_num_cells(GLuint num_cells_per_axis, GLuint index);

	~Noise();

private:

	void create_shader_programs();
	void generate_cells(GLuint num_cells_per_axis, GLuint cell_index);

	void generate_textures();
	void generate_res128_noise_texture();
	void generate_res32_noise_texture();

	void delete_textures();

	// first texture dim = 128^3
	GLuint texture_1_id;
	GLuint texture_dim_1;
	ComputeShaderProgram texture_1_shader_program;

	// 2nd texture dim = 32^3
	GLuint texture_2_id;
	GLuint texture_dim_2;
	ComputeShaderProgram texture_2_shader_program;

	GLuint						cell_ids[NUM_CELLS];
	GLuint						num_cells_per_axis[NUM_CELLS];
	std::shared_ptr<GLfloat[]>	cell_data [NUM_CELLS];

	DebugApp DebugApp_ins;

};

