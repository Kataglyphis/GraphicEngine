#pragma once
//lets mimic and c++-style interface :)
#include <glad/glad.h>

class TextureWrappingMode
{
public:
	TextureWrappingMode();

	virtual void activate() = 0;

	~TextureWrappingMode();

private:
};

