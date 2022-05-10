#pragma once
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include <vector>

#include "DirectionalLightUniformLocations.h"

#include "DebugApp.h"

class ShaderProgram
{
public: 

	ShaderProgram();

	ShaderProgram(const ShaderProgram&) = default;

	void create_from_files(const char* vertex_location, const char* fragment_location);
	void create_from_files(const char* vertex_location, const char* geometry_location, const char* fragment_location);
	void create_computer_shader_program_from_file(const char* compute_location);

	GLuint get_id();

	void use_shader_program();

	void validate_program();

	~ShaderProgram();

protected:

	std::string shader_base_dir;
	GLuint program_id;

	// the file locations from our shaders
	const char* vertex_location;
	const char* fragment_location;
	const char* geometry_location;
	const char* compute_location;

	void add_shader(GLuint program, const char* shader_code, GLenum shader_type);

	void compile_shader_program(const char* vertex_code, const char* fragment_code);
	void compile_shader_program(const char* vertex_code, const char* geometry_code, const char* fragment_code);
	void compile_compute_shader_program(const char * compute_code);
	void compile_program();

	virtual void retrieve_uniform_locations() = 0;

	void clear_shader_program();
	// for checking gl Errors
	DebugApp DebugApp_ins;

};
