#include <iostream>

#include "Terrain_texture.h"
#include "GeometryPassShaderProgram.h"
#include "glErrorChecker.h"

Terrain_Texture::Terrain_Texture() {
}


void Terrain_Texture::retreive_uniform_locations(GeometryPassShaderProgram* shader_program) {

	// check if they are errors before the function is executed
	if (glErrorChecker.arePreError("From set_unifomrs function in Terrain_texture.cpp")) {
		// do something?
	}

	// LOAD default texture
	biomHeight_id = shader_program->get_biom_height_id();
	isTerrainValue_id = shader_program->get_terrain_id();
	this->max_height_id = shader_program->get_max_height_id();

	for (int i = 0; i < NUM_BIOM_TEXTURES; i++) {
		glUniform1i(shader_program->get_biom_texture_location(i), i + 1);
	}


	// check with glErrorChecker.h if they are any glError and print it.
	std::string additionalMessage = "From set_uniforms in Terrain_texture.cpp file. \n";
	if (glErrorChecker.areErrorPrintAll(additionalMessage)) {
		// return;
	}

}


void Terrain_Texture::setHeights(std::vector<float> bio_heights, float max_height) {

	this-> max_height = max_height;


	if (bio_heights.size() != sizeof(biomHeights) / sizeof(*biomHeights)) {
		// bio_heights must have size of 8 
		std::cout << "bio_heights must have size of 8 " << std::endl;
		return;
	}
	
	for (int i = 0; i < bio_heights.size(); i++)
	{
		biomHeights[i] = bio_heights[i];
	}
}

bool Terrain_Texture::load_all_texture() {
	// load all texture
	std::vector<const char*> tex_paths = {	"Textures/Terrain/f-dark_water.jpg",
											"Textures/Terrain/f-shallow_water.jpg",
											"Textures/Terrain/f-adesert_sand2_d.jpg",
											"Textures/Terrain/f-grass_green_d.jpg",
											"Textures/Terrain/f-grass_ground_d.jpg",
											"Textures/Terrain/f-jungle_mntn2_d.jpg",
											"Textures/Terrain/f-mntn_white_d.jpg",
											"Textures/Terrain/f-snow2_s.jpg"
	};

	std::vector<Terrain_Texture::BIOMETYPE> biomtype = {	Terrain_Texture::BIOMETYPE::DEEP_WATER,
												Terrain_Texture::BIOMETYPE::SHALLOW_WATER,
												Terrain_Texture::BIOMETYPE::SAND,
												Terrain_Texture::BIOMETYPE::LIGHT_GRASS,
												Terrain_Texture::BIOMETYPE::DARK_GRASS,
												Terrain_Texture::BIOMETYPE::LIGHT_ROCK,
												Terrain_Texture::BIOMETYPE::DARK_ROCK,
												Terrain_Texture::BIOMETYPE::SNOW
	};

	for (int i = 0; i < biomtype.size(); i++)
	{
		if (!load_texture(_strdup(tex_paths[i]), biomtype[i])) {
			exit(EXIT_FAILURE);
		}
	}

	return true;
}

bool Terrain_Texture::load_texture(char* file_location, BIOMETYPE type) {

	// check if they are errors before the function is executed

	if (glErrorChecker.arePreError("Form load_texture in Terrain_texure.")) {
		// Error -> to something?
	}

	unsigned char* texture_data = stbi_load(file_location, &width[type], &height[type], &bit_depth[type], 0);
	if (!texture_data) {
		printf("Failed to find: %s\n", file_location);
		return false;
	}

	GLuint* Texture_type_ID = 0;
	switch (type)
	{
	case Terrain_Texture::DEEP_WATER:
		Texture_type_ID = &deep_water_texture_id;
		break;

	case Terrain_Texture::SHALLOW_WATER:
		Texture_type_ID = &water_texture_id;
		break;

	case Terrain_Texture::SAND:
		Texture_type_ID = &sand_texture_id;
		break;

	case Terrain_Texture::LIGHT_GRASS:
		Texture_type_ID = &light_gras_texture_id;
		break;

	case Terrain_Texture::DARK_GRASS:
		Texture_type_ID = &dark_gras_texture_id;
		break;

	case Terrain_Texture::LIGHT_ROCK:
		Texture_type_ID = &light_rock_texture_id;
		break;

	case Terrain_Texture::DARK_ROCK:
		Texture_type_ID = &dark_rock_texture_id;
		break;

	case Terrain_Texture::SNOW:
		Texture_type_ID = &snow_texture_id;
	default:
		break;
	}


	// generrate Texture
	glGenTextures(1, Texture_type_ID);
	glBindTexture(GL_TEXTURE_2D, *Texture_type_ID);
	wrapping_mode = new RepeatMode();
	wrapping_mode->activate();

	// i think we won't need nearest option; so stick to linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[type], height[type], 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texture_data);

	// check with glErrorChecker.h if they are any glError and print it.
	std::string additionalMessage = "From load_texture in Terrain_texture.cpp file. \n";
	additionalMessage += file_location;
	if (glErrorChecker.areErrorPrintAll(additionalMessage)) {
		// return;
	}

	return true;
}

void Terrain_Texture::use_texture() {


	// check if they are errors before the function is executed
	if (glErrorChecker.arePreError("Form use_texture function in Terrain_texure.")) {
		// Error -> to something?
	}


	glUniform1i(isTerrainValue_id, GL_TRUE);

	glUniform1fv(biomHeight_id, 8, biomHeights);

	glUniform1f(max_height_id, max_height);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, deep_water_texture_id);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, water_texture_id);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, sand_texture_id);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, light_gras_texture_id);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, dark_gras_texture_id);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, light_rock_texture_id);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, dark_rock_texture_id);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, snow_texture_id);
	/*		|
*		|
*		|
*		V
	This doesn`t work well because of GL_TEXTURE"X", where X between 1-8 ENUM.
*/
//for (int i = 0; i < sizeof(texture_Ids) / sizeof(*texture_Ids); i++)
//{
//	glActiveTexture(GL_TEXTURE8);
//	glBindTexture(GL_TEXTURE_2D, snow_texture_id);
//}


	// ---------------------------------TODO Got errer Message from here ---------------------------------------------
	// Woher kommt die meldung? GL_INVALID_OPERATION -> Parameter sind nicht valide -> ID von Tex oder  die ID der Uniform function
	
	// check with glErrorChecker.h if they are any glError and print it.
	std::string additionalMessage = "From use_texture function in Terrain_texture.cpp file. \n";
	if (glErrorChecker.areErrorPrintAll(additionalMessage)) {
		// return;
	}

}

void Terrain_Texture::unbind_texture() {

	glUniform1i(isTerrainValue_id, GL_FALSE);

	// is this enough or can i unbind all other Textures?
	//glBindTexture(GL_TEXTURE_2D, 0);

	// check with glErrorChecker.h if they are any glError and print it.
	std::string additionalMessage = "From unbind_texture function in Terrain_texture.cpp file. \n";
	if (glErrorChecker.areErrorPrintAll(additionalMessage)) {
		// return;
	}
	//glUseProgram(0);
	//glActiveTexture(0); // TODO Merge conflict DO i Need it?

}
void Terrain_Texture::clear_texture_context() {}

Terrain_Texture::~Terrain_Texture() {
	// release Texture allocation
	glDeleteTextures(1, &deep_water_texture_id);
	glDeleteTextures(1, &water_texture_id);
	glDeleteTextures(1, &sand_texture_id);
	glDeleteTextures(1, &light_gras_texture_id);
	glDeleteTextures(1, &dark_gras_texture_id);
	glDeleteTextures(1, &light_rock_texture_id);
	glDeleteTextures(1, &dark_rock_texture_id);
	glDeleteTextures(1, &snow_texture_id);
}