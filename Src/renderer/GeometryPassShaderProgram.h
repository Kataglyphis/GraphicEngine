#pragma once
#include "ShaderProgram.h"
#include <scene/ObjMaterialLocations.h>

class GeometryPassShaderProgram:
	public ShaderProgram
{

public:

	GeometryPassShaderProgram();

	GLuint get_projection_location();
	GLuint get_view_location();
	GLuint get_model_location();
	GLuint get_normal_modal_location();
	ObjMaterialLocations get_uniform_material_locations(int index);

	GLuint get_uniform_texture_locations(int index);

	GLuint get_program_id() { return program_id; }

	~GeometryPassShaderProgram();

protected:
	// the unique id of our program(program is set of ShaderPrograms ...)
	GLuint	uniform_model_location,
			uniform_view_location, uniform_projection_location,
			uniform_normal_model_location;

	GLuint uniform_texture_locations[MAX_TEXTURE_COUNT];

	ObjMaterialLocations uniform_material_locations[MAX_MATERIALS];

	void retrieve_uniform_locations();

};

