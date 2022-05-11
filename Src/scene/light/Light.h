#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CascadedShadowMap.h"

class Light
{
public:

	Light();
	Light(	GLfloat shadow_width, GLfloat shadow_height, 
			GLfloat red, GLfloat green, GLfloat blue,
			GLfloat radiance);


	~Light();

protected:

	glm::vec3	color;
	float		radiance;

	glm::mat4	light_proj;
};

