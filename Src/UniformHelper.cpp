#include "UniformHelper.h"

UniformHelper::UniformHelper()
{
}

bool UniformHelper::setUniformVec3(glm::vec3 uniform, GLuint uniform_location)
{
	bool validity = validateUniformLocation(uniform_location);
	if (validity) {
		glUniform3f(uniform_location, uniform.x, uniform.y, uniform.z);
	}
	return validity;
}

bool UniformHelper::setUniformFloat(GLfloat uniform, GLuint uniform_location)
{
	bool validity = validateUniformLocation(uniform_location);
	if (validity) {
		glUniform1f(uniform_location, uniform);
	}
	return validity;
}

bool UniformHelper::setUniformInt(GLint uniform, GLuint uniform_location)
{
	bool validity = validateUniformLocation(uniform_location);
	if (validity) {
		glUniform1i(uniform_location, uniform);
	}
	return validity;
}

bool UniformHelper::setUniformMatrix4fv(glm::mat4 matrix, GLuint uniform_location)
{
	bool validity = validateUniformLocation(uniform_location);
	if (validity) {
		glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	return validity;
}

UniformHelper::~UniformHelper()
{
}

bool UniformHelper::validateUniformLocation(GLuint uniformLocation)
{
	// if uniform location is invalid (f.e. var disappears because of optimizing of unused vars)
	return (uniformLocation == (GLint)-1) ? false : true;
}
