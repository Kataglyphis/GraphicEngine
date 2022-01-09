#include "Noise.h"

Noise::Noise() 
{

}

void Noise::init()
{

	this->texture_dim_1 = 128;
	this->texture_dim_2 = 32;
	texture_1_shader_program = ComputeShaderProgram();
	texture_2_shader_program = ComputeShaderProgram();
	texture_1_shader_program.create_computer_shader_program_from_file("Shaders/noise_texture_1.comp");
	texture_2_shader_program.create_computer_shader_program_from_file("Shaders/noise_texture_2.comp");

	for (int i = 0; i < NUM_CELLS; i++) {

		num_cells_per_axis[i] = pow(2, i+1);
		generate_cells(num_cells_per_axis[i], i);

	}

	generate_textures();

	/*int work_grp_cnt[3];

	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

	printf("max global (total) work group counts x:%i y:%i z:%i\n",
		work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

	int work_grp_size[3];

	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

	printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",
		work_grp_size[0], work_grp_size[1], work_grp_size[2]);*/

}

void Noise::generate_textures()
{
	glGenTextures(NUM_CELLS, cell_ids);
	
	for (int i = 0; i < NUM_CELLS; i++) {

		glBindTexture(GL_TEXTURE_3D, cell_ids[i]);
		// i think we won't need nearest option; so stick to linear
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, num_cells_per_axis[i], num_cells_per_axis[i], num_cells_per_axis[i], 0, GL_RGBA, GL_FLOAT, cell_data[i].get());
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_3D, 0);

	}

	glGenTextures(1, &texture_1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, texture_1);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, texture_dim_1, texture_dim_1, texture_dim_1, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_3D, 0);

	glGenTextures(1, &texture_2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, texture_2);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, texture_dim_2, texture_dim_2, texture_dim_2, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_3D, 0);

}

void Noise::delete_textures()
{

	glDeleteTextures(1, &texture_1);
	glDeleteTextures(1, &texture_2);
	
	for (int i = 0; i < NUM_CELLS; i++) {
	
		glDeleteTextures(NUM_CELLS, cell_ids);

	}

}

void Noise::update()
{

	delete_textures();

	texture_1_shader_program.reload();
	texture_2_shader_program.reload();

	for (int i = 0; i < NUM_CELLS; i++) {

		generate_cells(num_cells_per_axis[i], i);

	}

	generate_textures();

	// Check if any gl errorers appears.
	glErrorChecker_ins.areErrorPrintAll("From update function in Noise.cpp");

}

void Noise::set_num_cells(GLuint num_cells_per_axis, GLuint index)
{
	this->num_cells_per_axis[index] = num_cells_per_axis;
}

void Noise::generate_cells(GLuint num_cells_per_axis, GLuint cell_index)
{

	cell_data[cell_index] = std::shared_ptr<GLfloat[]>(new GLfloat[num_cells_per_axis* num_cells_per_axis* num_cells_per_axis * 4]);
	//GLfloat cell_size = 1.f / (GLfloat)num_cells_per_axis;

	std::mt19937_64 gen64 (25121995);
	std::uniform_real_distribution<float> dis(0, 1);
	
	for (int i = 0; i < static_cast<int>(num_cells_per_axis); i++) {
		for (int k = 0; k < static_cast<int>(num_cells_per_axis); k++) {
			for (int m = 0; m < static_cast<int>(num_cells_per_axis); m++) {

				GLfloat random_offset[3] = { dis(gen64), dis(gen64),dis(gen64)};
				GLfloat position[3] = {(i + random_offset[0]), (k + random_offset[1]), (m + random_offset[2])};

				GLuint index = (i + num_cells_per_axis * (k + m * num_cells_per_axis)) * 4;

				*(cell_data[cell_index].get() + index) = position[0];//
				*(cell_data[cell_index].get() + index + 1) = position[1];//
				*(cell_data[cell_index].get() + index + 2) = position[2];//
				*(cell_data[cell_index].get() + index + 3) = 1.0f;
			}
		}
	}

	//for (int i = 0; i < num_cells_per_axis; i++) {
	//	for (int k = 0; k < num_cells_per_axis; k++) {
	//		for (int m = 0; m < num_cells_per_axis; m++) {

	//			GLuint index = (i + num_cells_per_axis * (k + m * num_cells_per_axis)) * 4;
	//			printf("%f " , *(cell_data + index));//position[0];
	//			printf("%f ", *(cell_data + index + 1));; //position[1];
	//			printf("%f ",  *(cell_data + index + 2)); 
	//			printf("%f\n", *(cell_data + index + 3));
	//		}
	//	}
	//}
}

void Noise::create_worley_noise()
{
	
	texture_1_shader_program.use_shader_program();
	glBindImageTexture(0, texture_1, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	for (int i = 0; i < NUM_CELLS; i++) {

		glUniform1i(texture_1_shader_program.get_cell_location(i), i);
		glUniform1i(texture_1_shader_program.get_num_cell_location(i), num_cells_per_axis[i]);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_3D, cell_ids[i]);

	}

	glDispatchCompute((GLuint)texture_dim_1, (GLuint)texture_dim_1, (GLuint)texture_dim_1);
	
	// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glBindTexture(GL_TEXTURE_3D, 0);

	// Check if any gl errorers appears.
	glErrorChecker_ins.areErrorPrintAll("From create worley noise function in Noise.cpp");
}

void Noise::create_grad_noise()
{

	texture_2_shader_program.use_shader_program();
	glBindImageTexture(0, texture_2, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	for (int i = 0; i < NUM_CELLS; i++) {

		glUniform1i(texture_1_shader_program.get_cell_location(i), i);
		glUniform1i(texture_1_shader_program.get_num_cell_location(i), num_cells_per_axis[i]);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_3D, cell_ids[i]);

	}

	glDispatchCompute((GLuint)texture_dim_2, (GLuint)texture_dim_2, (GLuint)texture_dim_2);

	// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glBindTexture(GL_TEXTURE_3D, 0);

	// Check if any gl errorers appears.
	glErrorChecker_ins.areErrorPrintAll("From create grad noise function in Noise.cpp");

}

void Noise::read_worley_noise(GLenum start_buffer_index)
{
	GLuint texture_index = GL_TEXTURE0 + start_buffer_index;
	glActiveTexture((GLenum)texture_index);
	glBindTexture(GL_TEXTURE_3D, texture_1);

	// Check if any gl errorers appears.
	glErrorChecker_ins.areErrorPrintAll("From read worley noise function in Noise.cpp");
}

void Noise::read_grad_noise(GLenum start_buffer_index)
{
	GLuint texture_index = GL_TEXTURE0 + start_buffer_index;
	glActiveTexture((GLenum)texture_index);
	glBindTexture(GL_TEXTURE_3D, texture_2);

	// Check if any gl errorers appears.
	glErrorChecker_ins.areErrorPrintAll("From read grad noise function in Noise.cpp");
}

Noise::~Noise()
{
	delete_textures();
}
