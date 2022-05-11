#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include "GlobalValues.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "DebugApp.h"
#include "host_device_shared.h"

class CascadedShadowMap
{
public:

	CascadedShadowMap();

	virtual bool	init(GLuint width, GLuint height, GLuint num_cascades);
	virtual void	write();
	void			write_light_matrices(std::vector<glm::mat4x4>& lightMatrices);
	virtual void	read(GLenum texture_unit);
	void			set_pcf_radius(GLuint radius);
	void			set_intensity(GLfloat intensity);

	GLfloat			get_intensity();
	GLuint			get_shadow_width() { return shadow_width; };
	GLuint			get_shadow_height() { return shadow_height; }
	GLuint			get_id();
	GLuint			get_num_active_cascades();
	GLuint			get_pcf_radius();

	~CascadedShadowMap();

protected:

	GLuint	FBO, shadow_maps;

	GLuint	shadow_width, shadow_height;

	GLuint	matrices_UBO;

	GLuint	num_active_cascades;

	GLuint	pcf_radius;

	GLfloat intensity;

	DebugApp DebugApp_ins;
};

