#include "Clouds.h"

Clouds::Clouds()
{
}

void Clouds::init(GLfloat window_width, GLfloat window_height, GLuint movement_speed)
{
	model = glm::mat4(1.f);
	aabb = AABB();

	this->movement_speed = movement_speed;
	pillowness = 1.0;
	density = 0.7;
	cirrus_effect = 0.0f;
	powder_effect = true;

	minX = -1.f;  
	maxX = 1.f;
	minY = -1.f;
	maxY = 1.f;
	minZ = -1.f;
	maxZ = 1.f;

	aabb.init(minX, maxX, minY, maxY, minZ, maxZ);

	movement_direction = glm::vec3(0.0f,0.0f,1.0f);

	translation = glm::vec3(0.0f);
	scale_factor = glm::vec3(1.f);

	shader_program = new CloudsShaderProgram();

	shader_program->create_from_files("Shaders/Clouds.vert","Shaders/Clouds.frag");

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &cloud_id);
	glBindTexture(GL_TEXTURE_2D, cloud_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window_width, window_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cloud_id, 0);

	glDrawBuffers(1, attatchments);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Clouds::render(glm::mat4 projection_matrix, glm::mat4 view_matrix, GLfloat window_width, GLfloat window_height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window_width, window_height);

	shader_program->use_shader_program();
	retrieve_and_set_uniform_locations(projection_matrix, view_matrix);

	aabb.render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
}

void Clouds::read(GLuint index)
{
	GLuint texture_index = GL_TEXTURE0 +index;
	glActiveTexture(texture_index);
	glBindTexture(GL_TEXTURE_2D, cloud_id);
}

void Clouds::update_window_params(GLfloat window_width, GLfloat window_height)
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &cloud_id);
	glBindTexture(GL_TEXTURE_2D, cloud_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window_width, window_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cloud_id, 0);

	glDrawBuffers(1, attatchments);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

glm::mat4 Clouds::get_model()
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, translation);
	model = glm::scale(model, scale_factor);
	return model;
}

void Clouds::retrieve_and_set_uniform_locations(glm::mat4 projection_matrix, glm::mat4 view_matrix)
{

	glUniformMatrix4fv(shader_program->get_projection_location(), 1, GL_FALSE, glm::value_ptr(projection_matrix));
	glUniformMatrix4fv(shader_program->get_view_location(), 1, GL_FALSE, glm::value_ptr(view_matrix));

	glUniformMatrix4fv(shader_program->get_model_location(), 1, GL_FALSE, glm::value_ptr(get_model()));

	shader_program->validate_program();
}

glm::vec3 Clouds::get_movement_direction()
{
	return movement_direction;
}

glm::vec3 Clouds::get_rad()
{
	return glm::vec3(1.f);
}

glm::mat4 Clouds::get_normal_model()
{
	return glm::transpose(glm::inverse(model));
}

glm::vec3 Clouds::get_mesh_scale()
{
	return this->scale_factor;
}

GLfloat Clouds::get_movement_speed()
{
	return movement_speed / 10.f;
}

GLfloat Clouds::get_density()
{
	return density;
}

GLfloat Clouds::get_scale()
{
	return scale;
}

GLfloat Clouds::get_pillowness()
{
	return this->pillowness;
}

GLfloat Clouds::get_cirrus_effect()
{
	return this->cirrus_effect;
}

bool Clouds::get_powder_effect()
{
	return this->powder_effect;
}

void Clouds::set_movement_direction(glm::vec3 movement_dir)
{
	this->movement_direction = movement_dir;
}

CloudsShaderProgram* Clouds::get_shader_program()
{
	return shader_program;
}

Clouds::~Clouds()
{

}
