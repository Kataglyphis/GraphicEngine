#pragma once
#include <stdio.h>
#include <GL\glew.h>
#include "GlobalValues.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glErrorChecker.h"

class CascadedShadowMap
{
public:

	CascadedShadowMap();

	virtual bool init(GLuint width, GLuint height, GLuint num_cascades);
	virtual void write(GLuint cascade_index);
	virtual void read(GLenum texture_unit);
	void set_pcf_radius(GLuint radius);
	void set_intensity(GLfloat intensity);

	GLfloat get_intensity();
	GLuint get_shadow_width() { return shadow_width; };
	GLuint get_shadow_height() { return shadow_height; }
	GLuint get_id(GLuint cascade_index);
	GLuint get_num_active_cascades();
	GLuint get_pcf_radius();

	~CascadedShadowMap();

protected:

	GLuint FBO, shadow_map[NUM_MAX_CASCADES];
	GLuint shadow_width, shadow_height;

	GLuint num_active_cascades;

	GLuint pcf_radius;

	GLfloat intensity;

	glErrorChecker glErrorChecker;
};

