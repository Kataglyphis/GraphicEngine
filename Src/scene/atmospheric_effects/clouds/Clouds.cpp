#include "Clouds.h"

Clouds::Clouds() :

				shader_program(std::make_shared<ShaderProgram>(ShaderProgram{})),
				noise(std::make_shared<Noise>()),
				random_numbers(std::make_shared<RandomNumbers>()),
				model(glm::mat4(1.f)),
				aabb(AABB()),

				minX(- 1.f),
				maxX(1.f),
				minY(- 1.f),
				maxY(1.f),
				minZ(- 1.f),
				maxZ(1.f),

				movement_direction(glm::vec3(0.0f, 0.0f, 1.0f)),
				scale_factor(glm::vec3(1.f)),
				translation(glm::vec3(0.0f)),

				movement_speed(0.65f),
				density(0.7f),
				scale(0.63f),
				pillowness(1.0f),
				cirrus_effect(0.0f),

				num_march_steps(8),
				num_march_steps_to_light(3),

				powder_effect(true)

{

	shader_program->create_from_files(	"clouds/CloudsRectangle.vert",
										"clouds/CloudsRectangle.frag");

	create_noise_textures();

	aabb.init(minX, maxX, minY, maxY, minZ, maxZ);

}

void Clouds::render(glm::mat4 projection_matrix, glm::mat4 view_matrix, 
					GLfloat window_width, GLfloat window_height)
{

	shader_program->use_shader_program();
	shader_program->setUniformMatrix4fv(projection_matrix, "projection");
	shader_program->setUniformMatrix4fv(view_matrix, "view");
	shader_program->setUniformMatrix4fv(get_model(), "model");

	shader_program->validate_program();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	aabb.render();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	
	
}

void Clouds::read()
{

	random_numbers->read();
	noise->read_res128_noise();
	noise->read_res32_noise();

}

void Clouds::create_noise_textures()
{
	noise->create_res128_noise();
	noise->create_res32_noise();
}

void Clouds::set_powder_effect(bool cloud_powder_effect)
{
	this->powder_effect = cloud_powder_effect;
}

void Clouds::set_cirrus_effect(GLfloat cirrus_effect)
{
	this->cirrus_effect = cirrus_effect;
}

void Clouds::set_pillowness(GLfloat cloud_pillowness)
{
	this->pillowness = cloud_pillowness;
}

void Clouds::set_scale(GLfloat scale)
{
	this->scale = scale;
}

void Clouds::set_density(GLfloat density)
{
	this->density = density;
}

void Clouds::set_movement_speed(GLfloat speed)
{
	movement_speed = speed;
}

void Clouds::set_scale(glm::vec3 scale)
{
	scale_factor = scale;
}

void Clouds::set_translation(glm::vec3 translation)
{
	this->translation = translation;
}

glm::mat4 Clouds::get_model() const
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, translation);
	model = glm::scale(model, scale_factor);
	return model;
}

glm::vec3 Clouds::get_movement_direction() const
{
	return movement_direction;
}

glm::vec3 Clouds::get_rad() const
{
	return scale_factor / 2.f;
}

glm::mat4 Clouds::get_normal_model() const
{
	return glm::transpose(glm::inverse(model));
}

glm::vec3 Clouds::get_mesh_scale() const
{
	return this->scale_factor;
}

GLfloat Clouds::get_movement_speed() const
{
	return movement_speed / 10.f;
}

GLfloat Clouds::get_density() const
{
	return density;
}

GLfloat Clouds::get_scale() const
{
	return scale;
}

GLfloat Clouds::get_pillowness() const
{
	return this->pillowness;
}

GLfloat Clouds::get_cirrus_effect() const
{
	return this->cirrus_effect;
}

GLuint Clouds::get_num_march_steps() const
{
	return num_march_steps;
}

GLuint Clouds::get_num_march_steps_to_light() const
{
	return num_march_steps_to_light;
}

bool Clouds::get_powder_effect() const
{
	return this->powder_effect;
}

void Clouds::set_movement_direction(glm::vec3 movement_dir)
{
	this->movement_direction = movement_dir;
}

void Clouds::set_num_march_steps(GLuint num_march_steps)
{
	this->num_march_steps = num_march_steps;
}

void Clouds::set_num_march_steps_to_light(GLuint num_march_steps_to_light)
{
	this->num_march_steps_to_light = num_march_steps_to_light;
}

std::shared_ptr<ShaderProgram> Clouds::get_shader_program()
{
	return shader_program;
}

Clouds::~Clouds()
{
	
}
