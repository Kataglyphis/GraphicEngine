#pragma once
#include "AABB.h"
#include "CloudsShaderProgram.h"
#include "Noise.h"

#include <memory>

class Clouds
{
public:

	Clouds();

	void init(GLfloat window_width, GLfloat window_height);
	void render(glm::mat4 projection_matrix, glm::mat4 view_matrix, 
				GLfloat window_width, GLfloat window_height);
	void read(GLuint index);
	void update_window_params(GLfloat window_width, GLfloat window_height);

	void create_noise_textures();

	glm::mat4 get_model();
	glm::vec3 get_movement_direction();
	glm::vec3 get_rad();
	glm::mat4 get_normal_model();
	glm::vec3 get_mesh_scale();
	GLfloat get_movement_speed();
	GLfloat get_density();
	GLfloat get_scale();
	GLfloat get_pillowness();
	GLfloat get_cirrus_effect();
	bool get_powder_effect();

	void set_powder_effect(bool cloud_powder_effect);
	void set_cirrus_effect(GLfloat cirrus_effect);
	void set_pillowness(GLfloat cloud_pillowness);
	void set_scale(GLfloat scale);
	void set_density(GLfloat density);
	void set_movement_speed(GLfloat speed);
	void set_scale(glm::vec3 scale);
	void set_translation(glm::vec3 translation);
	void set_movement_direction(glm::vec3 movement_dir);

	std::shared_ptr<CloudsShaderProgram> get_shader_program();

	~Clouds();

private:

	void retrieve_and_set_uniform_locations(glm::mat4 projection_matrix, glm::mat4 view_matrix);

	GLuint FBO, cloud_id;

	glm::mat4 model;
	AABB aabb;
	GLfloat minX, maxX, minY, maxY, minZ, maxZ;

	glm::vec3 movement_direction;
	glm::vec3 scale_factor, translation;

	GLfloat movement_speed, density, scale, pillowness, cirrus_effect;

	bool powder_effect;

	std::shared_ptr<CloudsShaderProgram>	shader_program;
	std::shared_ptr<Noise>					noise;

	GLuint attatchments[1] = {GL_COLOR_ATTACHMENT0};
};

