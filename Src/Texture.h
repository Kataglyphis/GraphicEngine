#pragma once
#include <GL/glew.h>

#include <string.h>
#include "TextureWrappingMode.h"
#include "RepeatMode.h"
#include "MirroredRepeatMode.h"
#include "ClampToEdgeMode.h"

#include "GlobalValues.h"

#include "glErrorChecker.h"


class Texture
{
public:

	Texture();
	Texture(const char* file_loc, std::shared_ptr<TextureWrappingMode> wrapping_mode);

	bool load_texture_without_alpha_channel();
	bool load_texture_with_alpha_channel();
	std::string get_filename();

	void use_texture();
	void unbind_texture();
	void clear_texture_context();
	GLuint get_id();

	~Texture();

private:

	GLuint textureID;
	int width, height, bit_depth;

	std::shared_ptr<TextureWrappingMode> wrapping_mode;

	std::string file_location; //  Git merge conflict
	//const char* file_location; // //  Git merge conflict

	// this is for checking GL errors.
	glErrorChecker glErrorChecker_ins;
};

