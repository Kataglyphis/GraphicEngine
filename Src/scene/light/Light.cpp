#include "Light.h"

Light::Light() {

	color		= glm::vec3(1.0f);
	radiance	= float(1.0f);

}

Light::Light(GLfloat shadow_width, GLfloat shadow_height, 
					GLfloat red, GLfloat green, GLfloat blue,
					GLfloat radiance) {
	

	color = glm::vec3(red, green, blue);
	this->radiance = float(radiance);

}

Light::~Light() {

}