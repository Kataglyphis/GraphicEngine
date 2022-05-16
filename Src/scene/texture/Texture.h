#pragma once
#include <glad/glad.h>

#include <stb_image.h>
#include <string.h>
#include "TextureWrappingMode.h"
#include "RepeatMode.h"
#include "MirroredRepeatMode.h"
#include "ClampToEdgeMode.h"

#include <memory>
#include <string>

#include "GlobalValues.h"


class Texture
{
public:

	Texture();
	Texture(const char* file_loc, std::shared_ptr<TextureWrappingMode> wrapping_mode);

	bool load_texture_without_alpha_channel();
	bool load_texture_with_alpha_channel();

	bool load_SRGB_texture_without_alpha_channel();
	bool load_SRGB_texture_with_alpha_channel();

	std::string get_filename();
	GLuint		get_id();

	void use_texture(unsigned int index);
	void unbind_texture(unsigned int index);

	void clear_texture_context();


	~Texture();

private:

	GLuint textureID;
	int width, height, bit_depth;

	std::shared_ptr<TextureWrappingMode> wrapping_mode;

	std::string file_location;

};

