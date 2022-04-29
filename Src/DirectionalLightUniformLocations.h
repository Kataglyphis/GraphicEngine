#pragma once
#include "DirectionalLight.h"

struct  DirectionalLightUniformLocations {

	GLuint uniform_color_location;
	GLuint uniform_ambient_intensity_location;
	GLuint uniform_diffuse_intensity_location;

	GLuint uniform_view_location;

	GLuint uniform_direction_location;
	GLuint uniform_shadow_intensity_location;
};