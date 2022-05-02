#pragma once
#include <glm/glm.hpp> 

#ifndef COMMONVALS
#define COMMONVALS

#define NUM_MIN_CASCADES 3
#define MAX_RESOLUTION_X 1920
#define MAX_RESOLUTION_Y 1080

const int G_BUFFER_SIZE = 4;
const int NUM_FRUSTUM_PLANES = 6;
const int NUM_NOISE_TEXTURES = 2;
const int NUM_CELLS = 5;
const int NUM_CLOUDS = 1;

struct Rotation {

	GLfloat degrees;
	glm::vec3 axis;

};

#endif
