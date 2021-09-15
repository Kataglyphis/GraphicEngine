#pragma once
#include <iostream>
#include <vector>
#include  <unordered_map>

#include "Mesh.h"
#include "Vertex.h"
#include "AABB.h"

#include "GlobalValues.h"

class Model
{
public:

    Model();

    void load_model_in_ram(std::string model_path);

    void create_render_context();
    std::string get_base_dir(const std::string& filepath);
    AABB* get_aabb();

    void render();

    void transform_model(glm::vec3 translate_vec, glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f, glm::vec3 rotateAxis = glm::vec3(1.0f, 0.0f, 0.0f));

    ~Model();

private:

    AABB aabb;

    GLuint num_tex;

    std::vector<Mesh*> shapes;
    //std::vector<Mesh*> mesh_list;
    std::vector<Texture*> texture_list;

    std::vector<std::vector<Vertex>> vertices_per_shape;
    std::vector<std::vector<unsigned int>> indices_per_shape;

    std::vector<unsigned int>shapes_to_material;
    std::vector<int>material_to_tex;

    GLfloat minX = std::numeric_limits<float>::max();
    GLfloat maxX = std::numeric_limits<float>::min();
    GLfloat minY = std::numeric_limits<float>::max();
    GLfloat maxY = std::numeric_limits<float>::min();
    GLfloat minZ = std::numeric_limits<float>::max();
    GLfloat maxZ = std::numeric_limits<float>::min();

};
