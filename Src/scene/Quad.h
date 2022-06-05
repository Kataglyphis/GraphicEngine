#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>

#include "GlobalValues.h"

class Quad
{
public:

	Quad();

	void render();

	~Quad();

private:

	GLuint q_vao, q_vbo;

	float vertices[20] = {

		//positions		           //tex coords
		-1.0f,  1.0f, 0.0f,       0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,       0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,       1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,       1.0f, 0.0f

	};

};

