#pragma once
#include <string>

class File
{

public:

	File(std::string file_location);

	std::string read();

	~File();

private:

	std::string file_location;

};

