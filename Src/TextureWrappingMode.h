#pragma once
//lets mimic and c++-style interface :)
#include <GL/glew.h>

class TextureWrappingMode
{
public:
	TextureWrappingMode();

	virtual void activate() = 0;

	~TextureWrappingMode();

private:
};

