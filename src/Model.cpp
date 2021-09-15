#include "Model.h"

Model::Model()
{
    aabb = AABB();
    num_tex = 0;
}


std::string Model::get_base_dir(const std::string& filepath) {

    if (filepath.find_last_of("/\\") != std::string::npos)
        return filepath.substr(0, filepath.find_last_of("/\\"));
    return "";

}

AABB* Model::get_aabb()
{
    return &aabb;
}

void Model::load_model_in_ram(std::string model_path) {

    tinyobj::ObjReaderConfig reader_config;
    //reader_config.mtl_search_path = ""; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(model_path, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(EXIT_FAILURE);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // we now iterate over all materials to get diffuse textures
    {

        for (size_t i = 0; i < materials.size(); i++) {
            const tinyobj::material_t* mp = &materials[i];

            if (mp->diffuse_texname.length() > 0) {

                GLuint texture_id;
                int texture_width, texture_height;
                int comp;

                std::string relative_texture_filename = mp->diffuse_texname;
                std::string texture_filename = get_base_dir(model_path) + "/" + relative_texture_filename;



                texture_list.push_back(new Texture(texture_filename.c_str(), new RepeatMode()));

                /*if (!texture_list[num_tex]->load_texture_without_alpha_channel()) {
                    printf("Failed to load texture at: %s\n", texture_filename.c_str());
                    delete texture_list[i];
                    texture_list[i] = nullptr;
                }*/

                material_to_tex.push_back(num_tex);
                num_tex++;

            }
            else {

                material_to_tex.push_back(-1);

            }
        }

    }


    for (size_t s = 0; s < shapes.size(); s++) {

        std::unordered_map<Vertex, uint32_t> vertices_map{};
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (const auto& index : shapes[s].mesh.indices) {

            // access to vertex
            tinyobj::index_t idx = index;
            tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
            tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
            tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
            glm::vec3 pos = { vx,vy,vz };

            minX = std::min(minX, pos.x);
            maxX = std::max(maxX, pos.x);
            minY = std::min(minY, pos.y);
            maxY = std::max(maxY, pos.y);
            minZ = std::min(minZ, pos.z);
            maxZ = std::max(maxZ, pos.z);

            // Check if `normal_index` is zero or positive. negative = no normal data
            glm::vec3 normals(0.0f);
            if (idx.normal_index >= 0) {
                tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                normals = glm::vec3(nx, ny, nz);
            }

            glm::vec2 tex_coords(0.0f);
            // Check if `texcoord_index` is zero or positive. negative = no texcoord data
            if (idx.texcoord_index >= 0) {

                tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                tex_coords = glm::vec2(tx, ty);

            }

            Vertex vert(pos, normals, tex_coords);

            /* vertices.push_back(vert);
             indices.push_back(indices.size());*/

            if (vertices_map.count(vert) == 0) {

                vertices_map[vert] = vertices.size();
                vertices.push_back(vert);

            }

            indices.push_back(vertices_map[vert]);

        }

        vertices_per_shape.push_back(vertices);
        indices_per_shape.push_back(indices);

        shapes_to_material.push_back(shapes[s].mesh.material_ids[0]);
    }

}
void Model::create_render_context() {

    for (int i = 0; i < texture_list.size(); i++) {

        std::string texture_filename = texture_list[i]->get_filename();

        if (!texture_list[i]->load_texture_without_alpha_channel()) {
            printf("Failed to load texture at: %s\n", texture_filename.c_str());
            delete texture_list[i];
            texture_list[i] = nullptr;
        }

    }

    for (int i = 0; i < vertices_per_shape.size(); i++) {

        Mesh* result_mesh = new Mesh(vertices_per_shape[i], indices_per_shape[i]);
        this->shapes.push_back(result_mesh);

    }
        
    aabb.init(minX, maxX, minY, maxY, minZ, maxZ);
                                                                        
}

void Model::render()
{
    
    for (int i = 0; i < shapes.size(); i++) {

       if(material_to_tex[shapes_to_material[i]] >= 0) texture_list[material_to_tex[shapes_to_material[i]]]->use_texture();
       shapes[i]->render();
       if (material_to_tex[shapes_to_material[i]] >= 0) texture_list[material_to_tex[shapes_to_material[i]]]->unbind_texture();
    }

}


void Model::transform_model(glm::vec3 translate_vec, glm::vec3 scale, float angle, glm::vec3 rotateAxis) {
    for (int i = 0; i < shapes.size(); i++)
    {
        shapes[i]->transform_Mesh(translate_vec, scale, angle, rotateAxis);
    }
}


Model::~Model()
{
}



