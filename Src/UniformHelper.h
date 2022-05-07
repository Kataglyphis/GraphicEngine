#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class UniformHelper
{

public:

	UniformHelper();

	bool setUniformVec3(glm::vec3 uniform, GLuint uniform_location);
	bool setUniformFloat(GLfloat uniform, GLuint uniform_location);
	bool setUniformInt(GLint uniform, GLuint uniform_location);
	bool setUniformMatrix4fv(glm::mat4 matrix, GLuint uniform_location);
	bool setUniformBlockBinding(GLuint block_binding, GLuint uniform_location, GLuint shaderID);

	~UniformHelper();

private:

	bool validateUniformLocation(GLuint uniformLocation);

};

