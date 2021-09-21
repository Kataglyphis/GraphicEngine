#include "Light.h"

Light::Light() {

	color = glm::vec3(1.0f);
	ambient_intensity = float(1.0f);
	diffuse_intensity = float(0.0f);

}

Light::Light(GLfloat shadow_width, GLfloat shadow_height, 
					GLfloat red, GLfloat green, GLfloat blue,
					GLfloat a_intensity, GLfloat d_intensity) {
	

	color = glm::vec3(red, green, blue);
	ambient_intensity = float(a_intensity);
	diffuse_intensity = float(d_intensity);

}

Light::~Light() {

}