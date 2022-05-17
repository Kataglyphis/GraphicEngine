#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include "ShaderProgram.h"
#include "GlobalValues.h"
#include "bindings.h"

class GBuffer
{
public:

	GBuffer();
	GBuffer(GLint window_width, GLint window_height);

	void create();
	void read(std::shared_ptr<ShaderProgram> shader_program);
	
	void update_window_params(GLfloat window_width, GLfloat window_height);

	GLuint get_id() const { return g_buffer; };

	~GBuffer();

private: 

	GLuint g_buffer;

	GLuint g_position, g_normal, g_albedo, g_material_id, g_depth;

	GLuint g_buffer_attachment[G_BUFFER_SIZE] = 
					{	GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
						GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};

	GLint window_width, window_height;

};

