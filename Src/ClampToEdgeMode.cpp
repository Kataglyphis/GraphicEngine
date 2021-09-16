#include "ClampToEdgeMode.h"

ClampToEdgeMode::ClampToEdgeMode()
{
}

void ClampToEdgeMode::activate()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Check if an gl Error appears and print it.
	glErrorChecker_ins.areErrorPrintAll("From active() function in ClampToEdgeMode");
}

ClampToEdgeMode::~ClampToEdgeMode()
{
}
