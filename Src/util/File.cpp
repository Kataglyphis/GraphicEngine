#include "File.h"
#include <iostream>
#include <fstream>

File::File(const std::string& file_location) { this->file_location = file_location; }

std::string File::read()
{
  std::string content;
  std::ifstream file_stream(file_location, std::ios::in);

  if (!file_stream.is_open()) {
    printf("Failed to read %s. File does not exist.", file_location.c_str());
    return "";
  }

  std::string line = "";
  while (!file_stream.eof()) {
    std::getline(file_stream, line);
    content.append(line + "\n");
  }

  file_stream.close();
  return content;
}

File::~File() { }
