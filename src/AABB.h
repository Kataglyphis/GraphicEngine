#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "glErrorChecker.h"

class AABB
{
public: 

	AABB();

	std::vector<glm::vec3> get_corners(glm::mat4 model);
	void init(GLfloat minX,GLfloat maxX,GLfloat minY,GLfloat maxY,GLfloat minZ,GLfloat maxZ);
	void render();

	~AABB();

private:

	unsigned int VBO, VAO, EBO;

	unsigned int m_drawCount;

	std::vector<glm::vec3> corners;


	// This to test gl operators if they went correctly.
	glErrorChecker glErrorChecker;


	GLfloat minX, maxX, minY, maxY, minZ, maxZ;
};

