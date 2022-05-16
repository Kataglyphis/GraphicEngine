#pragma once
#include <memory>

#include "AABB.h"
#include "ShaderProgram.h"
#include "Noise.h"
#include "RandomNumbers.h"

class Clouds
{
public:

	Clouds();

	void		render(	glm::mat4 projection_matrix, glm::mat4 view_matrix, 
						GLfloat window_width, GLfloat window_height);

	void		read();

	void		create_noise_textures();

	glm::mat4	get_model();
	glm::vec3	get_movement_direction();
	glm::vec3	get_rad();
	glm::mat4	get_normal_model();
	glm::vec3	get_mesh_scale();
	GLfloat		get_movement_speed();
	GLfloat		get_density();
	GLfloat		get_scale();
	GLfloat		get_pillowness();
	GLfloat		get_cirrus_effect();
	GLuint		get_num_march_steps();
	GLuint		get_num_march_steps_to_light();
	bool		get_powder_effect();

	void		set_powder_effect(bool cloud_powder_effect);
	void		set_cirrus_effect(GLfloat cirrus_effect);
	void		set_pillowness(GLfloat cloud_pillowness);
	void		set_scale(GLfloat scale);
	void		set_density(GLfloat density);
	void		set_movement_speed(GLfloat speed);
	void		set_scale(glm::vec3 scale);
	void		set_translation(glm::vec3 translation);
	void		set_movement_direction(glm::vec3 movement_dir);
	void		set_num_march_steps(GLuint num_march_steps);
	void		set_num_march_steps_to_light(GLuint num_march_steps_to_light);

	std::shared_ptr<ShaderProgram> get_shader_program();

	~Clouds();

private:

	glm::mat4 model;
	AABB aabb;
	GLfloat minX, maxX, minY, maxY, minZ, maxZ;

	glm::vec3 movement_direction;
	glm::vec3 scale_factor, translation;

	GLfloat movement_speed, density, scale, pillowness, cirrus_effect;

	GLuint	num_march_steps, num_march_steps_to_light;

	bool powder_effect;

	std::shared_ptr<ShaderProgram>	shader_program;
	std::shared_ptr<Noise>			noise;
	std::shared_ptr<RandomNumbers>	random_numbers;

};

