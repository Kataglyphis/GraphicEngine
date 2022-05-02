#include "CascadedShadowMap.h"

CascadedShadowMap::CascadedShadowMap()
{
	FBO = 0;
	for (unsigned int i = 0; i < NUM_CASCADES; i++) {
		shadow_map[i] = 0;
	}
	pcf_radius = 1;
}

bool CascadedShadowMap::init(GLuint width, GLuint height, GLuint num_cascades)
{
	DebugApp_ins.arePreError("From init() function in CascadedShadowMap.");

	shadow_width = width;
	shadow_height = height;

	num_active_cascades = num_cascades;

	glGenFramebuffers(1, &FBO);

	glGenTextures(num_cascades, shadow_map);

	for (unsigned int i = 0; i < num_cascades; i++) {

		glBindTexture(GL_TEXTURE_2D, shadow_map[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

	}

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map[0], 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {

		printf("Framebuffer error: %i\n", status);
		return false;

	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (DebugApp_ins.areErrorPrintAll("From int() function in CascadedShadowMap.")) {
		// DO something?
	}

	return true;
}

void CascadedShadowMap::write(GLuint cascade_index)
{

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map[cascade_index], 0);

}

void CascadedShadowMap::read(GLenum texture_unit)
{
	for (size_t i = 0; i < NUM_CASCADES; i++) {

		glActiveTexture(GL_TEXTURE0 + texture_unit + i);
		glBindTexture(GL_TEXTURE_2D, shadow_map[i]);

	}
}

void CascadedShadowMap::set_pcf_radius(GLuint radius)
{
	pcf_radius = radius;
}

void CascadedShadowMap::set_intensity(GLfloat intensity)
{
	this->intensity = intensity;
}

GLfloat CascadedShadowMap::get_intensity()
{
	return intensity;
}

GLuint CascadedShadowMap::get_id(GLuint cascade_index)
{
	return shadow_map[cascade_index];
}

GLuint CascadedShadowMap::get_num_active_cascades()
{
	return num_active_cascades;
}

GLuint CascadedShadowMap::get_pcf_radius()
{
	return pcf_radius;
}

CascadedShadowMap::~CascadedShadowMap()
{
	if (FBO) {
		glDeleteFramebuffers(1, &FBO);
	}

	if (shadow_map) {
		glDeleteTextures(NUM_CASCADES, shadow_map);
	}
}
