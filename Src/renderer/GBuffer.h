#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GlobalValues.h"
#include "texture_unit_slots.h"
#include "UniformHelper.h"

class GBuffer
{
public:

	GBuffer();
	GBuffer(GLint window_width, GLint window_height);

	void create();

	void read(	GLuint g_buffer_lighting_uniform_position_location,
				GLuint g_buffer_lighting_uniform_normal_location,
				GLuint g_buffer_lighting_uniform_albedo_location,
				GLuint g_buffer_frag_depth_location,
				GLuint g_buffer_material_id_location);
	
	void update_window_params(GLfloat window_width, GLfloat window_height);

	GLint get_id();

	~GBuffer();

private: 

	GLuint g_buffer;

	GLuint g_position, g_normal, g_albedo, g_depth, g_fragment_depth,
		g_material_id;

	GLuint g_buffer_attachment[G_BUFFER_SIZE] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
														GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
														GL_COLOR_ATTACHMENT4};

	GLint window_width, window_height;
	UniformHelper uniform_helper;

};

