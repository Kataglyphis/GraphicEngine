#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "Texture.h"

Texture::Texture()
{
	textureID			= 0;
	width				= 0;
	height				= 0;
	bit_depth			= 0;
	file_location		= strdup("");
	//go with reapeat as standard ...
	this->wrapping_mode = std::make_shared<RepeatMode>();
}

Texture::Texture(const char* file_loc, std::shared_ptr<TextureWrappingMode> wrapping_mode)
{
	textureID			= 0;
	width				= 0;
	height				= 0;
	bit_depth			= 0;
	file_location		= std::string(file_loc);
	this->wrapping_mode = wrapping_mode;
}

bool Texture::load_texture_without_alpha_channel()
{

	// check if they are errors before the function is executed
	DebugApp_ins.arePreError("From load_texture_without_alpha_channel in Texture.cpp file.");

	stbi_set_flip_vertically_on_load(true);
	unsigned char* texture_data = stbi_load(file_location.c_str(), &width, &height, &bit_depth, 0);
	if (!texture_data) {
		printf("Failed to find: %s\n", file_location.c_str());
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	wrapping_mode->activate();
	
	// i think we won't need nearest option; so stick to linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// INVALID ENUM changed to GL_LINEAR 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texture_data);

	// check with DebugApp_ins.h if they are any glError and print it.
	std::string additionalMessage = "From load_texture_without_alpha_channel in Texture.cpp file. \n";
	additionalMessage += file_location;
	if (DebugApp_ins.areErrorPrintAll(additionalMessage)) {
		// return false;
	}

	return true;
}

bool Texture::load_texture_with_alpha_channel()
{
	// check if they are errors before the function is executed
	DebugApp_ins.arePreError("From load_texture_with_alpha_channel in Texture.cpp file.");
	stbi_set_flip_vertically_on_load(true);
	unsigned char* texture_data = stbi_load(file_location.c_str(), &width, &height, &bit_depth, 0);
	if (!texture_data) {
		printf("Failed to find: %s\n", file_location.c_str());
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	wrapping_mode->activate();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// to not interpolate between transparent and not trans coloars
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// INVALID ENUM changed to GL_LINEAR 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texture_data);


	// check with DebugApp_ins.h if they are any glError and print it.
	std::string additionalMessage = "From load_texture_with_alpha_channel in Texture.cpp file. \n";
	additionalMessage += file_location;
	if (DebugApp_ins.areErrorPrintAll(additionalMessage)) {
		// return false;
	}


	return true;
}

bool Texture::load_SRGB_texture_without_alpha_channel()
{
	// check if they are errors before the function is executed
	DebugApp_ins.arePreError("From load_texture_without_alpha_channel in Texture.cpp file.");

	stbi_set_flip_vertically_on_load(true);
	unsigned char* texture_data = stbi_load(file_location.c_str(), &width, &height, &bit_depth, 0);
	if (!texture_data) {
		printf("Failed to find: %s\n", file_location.c_str());
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	wrapping_mode->activate();

	// i think we won't need nearest option; so stick to linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// INVALID ENUM changed to GL_LINEAR 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texture_data);

	// check with DebugApp_ins.h if they are any glError and print it.
	std::string additionalMessage = "From load_texture_without_alpha_channel in Texture.cpp file. \n";
	additionalMessage += file_location;
	if (DebugApp_ins.areErrorPrintAll(additionalMessage)) {
		// return false;
	}

	return true;
}

bool Texture::load_SRGB_texture_with_alpha_channel()
{
	// check if they are errors before the function is executed
	DebugApp_ins.arePreError("From load_texture_with_alpha_channel in Texture.cpp file.");
	stbi_set_flip_vertically_on_load(true);
	unsigned char* texture_data = stbi_load(file_location.c_str(), &width, &height, &bit_depth, 0);
	if (!texture_data) {
		printf("Failed to find: %s\n", file_location.c_str());
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	wrapping_mode->activate();


	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// to not interpolate between transparent and not trans coloars
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// INVALID ENUM changed to GL_LINEAR 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texture_data);


	// check with DebugApp_ins.h if they are any glError and print it.
	std::string additionalMessage = "From load_texture_with_alpha_channel in Texture.cpp file. \n";
	additionalMessage += file_location;
	if (DebugApp_ins.areErrorPrintAll(additionalMessage)) {
		// return false;
	}


	return true;
}

std::string Texture::get_filename()
{
	return file_location;
}

void Texture::use_texture(unsigned int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// check with DebugApp_ins.h if they are any glError and print it.
	if (DebugApp_ins.areErrorPrintAll("From use_texture in Texture.cpp file.")) {
		// return false;
	}
}

void Texture::unbind_texture(unsigned int index) {
	glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + index);
}

void Texture::clear_texture_context()
{
	glDeleteTextures(1, &textureID);
	textureID		= 0;
	width			= 0;
	height			= 0;
	bit_depth		= 0;
	file_location	= strdup("");
}

GLuint Texture::get_id()
{
	return textureID;
}

Texture::~Texture()
{
	clear_texture_context();
}
