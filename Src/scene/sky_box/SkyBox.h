#pragma once
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GlobalValues.h"

#include "Mesh.h"
#include "Vertex.h"
#include "SkyBoxShaderProgram.h"

#include <sstream>

#include <ctime>
#include <chrono>
#include <random>
#include <cassert>
#include <time.h>  

#include "bindings.h"


#include "DebugApp.h"

class SkyBox
{
public:
	SkyBox();

	SkyBox(std::vector<std::string> face_locations);

	void draw_sky_box(	glm::mat4 projection_matrix, glm::mat4 view_matrix, 
						GLfloat window_width, GLfloat window_height, 
						GLfloat delta_time);

	void reload();

	~SkyBox();

private:

	GLfloat movement_speed = 0.1f;

	GLfloat shader_playback_time;

	std::shared_ptr<Mesh> sky_mesh;
	std::shared_ptr<SkyBoxShaderProgram> shader_program;

	GLuint texture_id;
	GLuint uniform_projection, uniform_view;

	DebugApp		DebugApp_ins;

};

