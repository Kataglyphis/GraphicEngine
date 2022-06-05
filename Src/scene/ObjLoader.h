#pragma once
#include <memory>
#include <stdexcept>

#include "Mesh.h"
#include "Texture.h"
#include "Vertex.h"
#include "ObjMaterial.h"
#include "host_device_shared.h"

class ObjLoader {
  public:
  ObjLoader();

  void load(std::string modelFile, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<std::string>& texture_list,
    std::vector<ObjMaterial>& materials, std::vector<glm::vec4>& materialIndex);

  ~ObjLoader();

  private:
  GLfloat minX = std::numeric_limits<float>::max();
  GLfloat maxX = std::numeric_limits<float>::min();
  GLfloat minY = std::numeric_limits<float>::max();
  GLfloat maxY = std::numeric_limits<float>::min();
  GLfloat minZ = std::numeric_limits<float>::max();
  GLfloat maxZ = std::numeric_limits<float>::min();

  static std::string get_base_dir(const std::string& filepath)
  {

    if (filepath.find_last_of("/\\") != std::string::npos) return filepath.substr(0, filepath.find_last_of("/\\"));
    return "";
  }
};
