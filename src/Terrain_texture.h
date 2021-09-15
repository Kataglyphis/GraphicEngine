#pragma once

#include <string>
#include "GeometryPassShaderProgram.h"

#include "Texture.h"
#include "glErrorChecker.h"

class Terrain_Texture {

public:
	Terrain_Texture();

	enum BIOMETYPE
	{
		DEEP_WATER,
		SHALLOW_WATER,
		SAND,
		LIGHT_GRASS,
		DARK_GRASS,
		LIGHT_ROCK,
		DARK_ROCK,
		SNOW
	};

	bool load_texture(char * file_location,BIOMETYPE type);
	
	void retreive_uniform_locations(GeometryPassShaderProgram* shader_program);

	void use_texture();
	void unbind_texture();
	void setHeights(std::vector<float> heights, float max_height);
	void clear_texture_context();
	bool load_all_texture();


	~Terrain_Texture();
private:
	GLuint deep_water_texture_id;
	GLuint water_texture_id;
	GLuint sand_texture_id;
	GLuint light_gras_texture_id;
	GLuint dark_gras_texture_id;
	GLuint light_rock_texture_id;
	GLuint dark_rock_texture_id;
	GLuint snow_texture_id;
	/* gather all texture ids here ? -> GL_TEXTURE"X" enum later
				|
				|
				V                       */
	//GLuint texture_Ids[8];

	

	GLuint isTerrainValue_id;
	GLuint biomHeight_id;
	float biomHeights[8];
	GLuint max_height_id;
	float max_height;

	int width[8], height[8], bit_depth[8];

	TextureWrappingMode* wrapping_mode;

	char* file_location;
	glErrorChecker glErrorChecker;

	// Hard coded Program ID
	GLuint program_id;
};