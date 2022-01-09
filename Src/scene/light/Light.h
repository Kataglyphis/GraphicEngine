#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CascadedShadowMap.h"

class Light
{
public:

	Light();
	Light(GLfloat shadow_width, GLfloat shadow_height, 
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat a_intensity, GLfloat d_intensity);


	~Light();

protected:

	glm::vec3 color;
	float ambient_intensity;
	float diffuse_intensity;

	glm::mat4 light_proj;
};

