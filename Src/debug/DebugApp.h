#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

class DebugApp
{
public:

	DebugApp();

	bool areErrorPrintAll(	std::string AdditionalArrayMessage = "Empty", 
							const char* file = __FILE__, 
							int line = __LINE__);

	bool arePreError(	std::string AdditionalArrayMessage = "Empty", 
						const char* file = __FILE__, 
						int line = __LINE__);

	~DebugApp();

private:

};

