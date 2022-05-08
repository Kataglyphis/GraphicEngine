#pragma once
#include <vector>

class ShaderIncludes
{

public:

	ShaderIncludes();

	void set(	std::vector<const char*> includeNames,
				std::vector<const char*> file_locations_relative);

	~ShaderIncludes();

private:

};

