#pragma once
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <random>

#include "GlobalValues.h"

class RandomNumbers
{

public:

	RandomNumbers();

	void read();

	~RandomNumbers();

private:

	GLuint						random_number_id;
	std::shared_ptr<GLfloat[]>	random_number_data;
	std::shared_ptr<GLfloat[]>	generate_random_numbers();
};

