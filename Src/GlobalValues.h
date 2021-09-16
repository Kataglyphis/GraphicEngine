#pragma once

#ifndef COMMONVALS
#define COMMONVALS
#include <stb_image.h>
#include <tiny_obj_loader.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define NUM_MIN_CASCADES 3
#define NUM_MAX_CASCADES 10
#define NUM_BIOM_TEXTURES 8
#define MAX_RESOLUTION_X 1920
#define MAX_RESOLUTION_Y 1080

const int MAX_POINT_LIGHTS = 3;
const int MAX_MATERIALS = 2;
const int G_BUFFER_SIZE = 5;
const int NUM_FRUSTUM_PLANES = 6;
const int NUM_NOISE_TEXTURES = 2;
const int NUM_CELLS = 5;
const int NUM_CLOUDS = 1;

struct Rotation {

	GLfloat degrees;
	glm::vec3 axis;

};

#endif
