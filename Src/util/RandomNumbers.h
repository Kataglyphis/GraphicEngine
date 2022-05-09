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

	std::shared_ptr<GLfloat[]> generate_random_numbers();

	~RandomNumbers();

private:

};

