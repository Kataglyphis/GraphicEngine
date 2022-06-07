#pragma once
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "AABB.h"
#include "GlobalValues.h"
#include "Mesh.h"
#include "ObjLoader.h"
#include "ObjMaterial.h"
#include "Vertex.h"
#include "bindings.h"

class Model {
 public:
  Model();

  void load_model_in_ram(const std::string& model_path);

  void create_render_context();

  void bind_ressources();

  void unbind_resources();

  std::shared_ptr<AABB> get_aabb();
  std::vector<ObjMaterial> get_materials() const;
  int get_texture_count() const;

  void render();

  ~Model();

 private:
  // buffer for material id's
  GLuint ssbo;

  ObjLoader loader;

  std::shared_ptr<AABB> aabb;

  std::shared_ptr<Mesh> mesh;
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<Texture>> texture_list;
  std::vector<ObjMaterial> materials;
  std::vector<glm::vec4> materialIndex;
  std::vector<std::string> textures;
};
