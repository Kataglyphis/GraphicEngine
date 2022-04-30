#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

#include "Mesh.h"
#include "Vertex.h"
#include "AABB.h"

#include <memory>

#include "GlobalValues.h"
#include "ObjMaterial.h"
#include "ObjLoader.h"

class Model
{
public:

    Model();

    void load_model_in_ram(std::string model_path);

    void create_render_context();

    std::shared_ptr<AABB>       get_aabb();
    std::vector<ObjMaterial>    get_materials();

    void render();

    ~Model();

private:

    // buffer for material id's
    GLuint ssbo;

    ObjLoader loader;

    std::shared_ptr<AABB> aabb;

    std::shared_ptr<Mesh>                   mesh;
    std::vector<Vertex>			            vertices;
    std::vector<unsigned int>	            indices;
    std::vector<std::shared_ptr<Texture>>   texture_list;
    std::vector<ObjMaterial>	            materials;
    std::vector<unsigned int>	            materialIndex;
    std::vector<std::string>	            textures;
};
