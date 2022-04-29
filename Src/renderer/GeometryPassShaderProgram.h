#pragma once
#include "ShaderProgram.h"

class GeometryPassShaderProgram:
	public ShaderProgram
{

public:

	GeometryPassShaderProgram();

	GLuint get_projection_location();
	GLuint get_view_location();
	GLuint get_model_location();
	GLuint get_normal_modal_location();
	GLuint get_material_id_location();
	GLuint get_terrain_id();
	GLuint get_biom_height_id();
	GLuint get_max_height_id();

	// change this by kansei is it okay?
	GLuint get_program_id() { return program_id; }

	~GeometryPassShaderProgram();

protected:
	// the unique id of our program(program is set of ShaderPrograms ...)
	GLuint uniform_model_location,
		uniform_view_location, uniform_projection_location,
		uniform_material_id_location, uniform_normal_model_location;

	GLuint uniform_isTerrainValue_id, uniform_biomHeight_id,
					uniform_max_height_id;

	void retrieve_uniform_locations();

};

