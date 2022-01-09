#pragma once

#include <stdio.h>
#include <GL/glew.h>

class ShadowMap
{
public:

	ShadowMap();

	virtual bool init(GLuint width, GLuint height);

	virtual void write();

	virtual void read(GLenum texture_unit);

	GLuint get_shadow_width() { return shadow_width; };

	GLuint get_shadow_height() { return shadow_height; };

	GLuint get_id();

	~ShadowMap();

protected:

	GLuint FBO, shadow_map;
	GLuint shadow_width, shadow_height;
};

