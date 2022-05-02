#include "SkyBox.h"

SkyBox::SkyBox()
{
}

SkyBox::SkyBox(std::vector<std::string> face_locations)
{

	// check if there any other gl Error  appears before execue gl functions
	DebugApp_ins.arePreError("From SkyBox constructor in SkyBox.cpp file.");

	uniform_helper = UniformHelper();

	srand(time(NULL));
	shader_playback_time = 1;

	sky_shader_program = std::make_shared<SkyBoxShaderProgram>();
	sky_shader_program->create_from_files("skybox/SkyBox.vert", "skybox/SkyBox.frag");

	uniform_projection = sky_shader_program->get_projection_location();
	uniform_view = sky_shader_program->get_view_location();

	//texture setup
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	int width, height, bit_depth;

	for (size_t i = 0; i < 6; i++) {

		unsigned char* texture_data = stbi_load(face_locations[i].c_str(), &width, &height, &bit_depth, 0);
		if (!texture_data) {
			printf("Failed to find: %s\n", face_locations[i].c_str());
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		stbi_image_free(texture_data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// check with DebugApp_ins.h if they are any glError and print it.
	if (DebugApp_ins.areErrorPrintAll("From SkyBox constructor in SkyBox.cpp file.")) {
		// return false;
	}

	// Mesh Setup
	std::vector<unsigned int> sky_box_indices = {
		//front
		0,1,2,
		2,1,3,
		//right
		2,3,5,
		5,3,7,
		//back
		5,7,4,
		4,7,6,
		//left
		4,6,0,
		0,6,1,
		//top
		4,0,5,
		5,0,2,
		//bottom
		1,6,3,
		3,6,7
	};

	std::vector<Vertex> sky_box_vertices = {

		Vertex(glm::vec3(-1.0f, 1.0f, -1.0f)	, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)	, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(1.0f, 1.0f, -1.0f)		, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(1.0f, -1.0f, -1.0f)	, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec2(0.0f, 0.0f)),
																		   
		Vertex(glm::vec3(-1.0f, 1.0f, 1.0f)		, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(1.0f, 1.0f, 1.0f)		, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, 1.0f)	, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(1.0f, -1.0f, 1.0f)		, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec2(0.0f, 0.0f)),
				
	};

	sky_mesh = std::make_shared<Mesh>(sky_box_vertices, sky_box_indices);

}

void SkyBox::draw_sky_box(	glm::mat4 projection_matrix, glm::mat4 view_matrix, GLfloat window_width,
							GLfloat window_height, GLfloat delta_time, GLuint skyBoxMatreialID)
{
	// check if there any other gl Error  appears before execue gl functions
	DebugApp_ins.arePreError("From draw_sky_box function in SkyBox.cpp file.");


 	GLfloat velocity = movement_speed * delta_time;
	shader_playback_time = fmod(shader_playback_time + velocity, 10000);

	glm::mat4 new_view_matrix = glm::mat4(glm::mat3(view_matrix));

	glDepthMask(GL_FALSE);

	//std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	sky_shader_program->use_shader_program();

	uniform_helper.setUniformInt(skyBoxMatreialID, sky_shader_program->get_uniform_skyBoxMaterialID_location());
	uniform_helper.setUniformMatrix4fv(projection_matrix, uniform_projection);
	uniform_helper.setUniformMatrix4fv(new_view_matrix, uniform_view);
	uniform_helper.setUniformInt(SKYBOX_TEXTURES_SLOT, sky_shader_program->get_uniform_samplerCube_location());

	glActiveTexture(GL_TEXTURE0 + SKYBOX_TEXTURES_SLOT);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	sky_shader_program->validate_program();

	sky_mesh->render();

	glDepthMask(GL_TRUE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	// check with DebugApp_ins.h if they are any glError and print it.
	if (DebugApp_ins.areErrorPrintAll("From draw_sky_box function in SkyBox.cpp file.")) {
		// do something?
	}

}

void SkyBox::reload()
{
	sky_shader_program = std::make_shared<SkyBoxShaderProgram>();
	sky_shader_program->create_from_files("Shaders/SkyBox.vert", "Shaders/SkyBox.frag");

	uniform_projection = sky_shader_program->get_projection_location();
	uniform_view = sky_shader_program->get_view_location();
}

SkyBox::~SkyBox()
{

}
