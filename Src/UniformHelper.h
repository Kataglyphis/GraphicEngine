#pragma once
#include <glew.h>
#include <glm/glm.hpp>

class UniformHelper
{

public:

	UniformHelper();

	bool setUniformVec3(glm::vec3 uniform, GLuint uniform_location);
	bool setUniformFloat(GLfloat uniform, GLuint uniform_location);
	bool setUniformInt(GLint uniform, GLuint uniform_location);

	~UniformHelper();

private:

	bool validateUniformLocation(GLuint uniformLocation);

};

