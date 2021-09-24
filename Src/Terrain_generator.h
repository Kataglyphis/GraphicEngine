#pragma once

#include <vector>
#include <glew.h>
#include <tuple>
#include <glm/glm.hpp>
#include <limits>
#include "RenderPassSceneDependend.h"
#include "ViewFrustumCulling.h"

#include <GL/glew.h>

#include "Mesh.h"
#include "Texture.h"
#include "Terrain_Model.h"
#include "Model.h"
#include "Terrain_texture.h"
#include "ShaderProgram.h"
#include "AABB.h"
#include "GeometryPassShaderProgram.h"

#include "glErrorChecker.h"



class Terrain_Generator
{
public:

	Terrain_Generator();

	void init();
	void retreive_uniform_locations(std::shared_ptr<GeometryPassShaderProgram> shader_program);

	~Terrain_Generator();

	void set_world_trafo(glm::mat4 model_to_world);
	glm::mat4 get_world_trafo();
	glm::mat4 get_normal_world_trafo();
	void set_material_id(GLuint material_id);
	GLuint get_material_id();

	std::shared_ptr<Terrain_Texture> get_textures();

	void generateMesh(GLfloat& progress);
	void load_plants();

	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<AABB>> aabbs;

	// TODO:
	void expandTerrain();

	std::vector<GLuint> generate_indices();
	std::vector<float> generate_noise_map(int xOffset, int yOffset);
	std::vector<glm::vec3> generate_vertices(const std::vector<float>& noise_map);

	std::vector<glm::vec3> calc_smooth_normals(std::vector<glm::vec3> verticesPos, std::vector<glm::vec3> triangleNormals, std::vector<unsigned int> indices);
	std::vector<glm::vec3> generate_normals(const std::vector<GLuint>& indices, const std::vector<glm::vec3>& vertices);

	
	void generate_map_chunk(/*GLuint& VAO,*/ int xOffset, int yOffset , std::vector< std::vector<Terrain_Model>>& chunk_models);

	std::vector<glm::vec2> generate_biome(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices,  int xOffset, int yOffset, std::vector< std::vector<Terrain_Model>>& chunk_models);

	void generate_render_context();

	/// dynamic render Terrain TODO paralell loading?
	// Parameter:
	// view: is the camera position (GetViewMatrix)	
	// model:
	// projection: 
	std::vector<bool> render(GLfloat ratio, std::shared_ptr<Camera> camera, std::shared_ptr<ViewFrustumCulling> view_cull, RenderPassSceneDependend* shader_pass);

	void render_plants(std::vector<bool> is_chunk_rendered_flags, RenderPassSceneDependend* shader_pass);

	void transform(glm::vec3 translate_vector, glm::vec3 scale);

	void changeMaxHeight(float newMaxHeight);
	void regenerate();


	// get noise Parameters  return (octaves, meshHeight, noiseScale, persistence, lacunarity)
	std::tuple<int, float, float, float, float> getNoiseParameters() {
		return { octaves, meshHeight, noiseScale, persistence, lacunarity };
	}

	// Set noise Parameters set(octaves, meshHeight, noiseScale, persistence, lacunarity)
	void setNoiseParameters(std::tuple<int, float, float, float, float> noiseParam) {
		this->octaves = std::get<0>(noiseParam);
		this->meshHeight = std::get<1>(noiseParam);
		this->noiseScale = std::get<2>(noiseParam);
		this->persistence = std::get<3>(noiseParam);
		this->lacunarity = std::get<4>(noiseParam);

		regenerate();
	}

private:
	// Terrain Map params 
	float WATER_HEIGHT = 0.1;
	int chunk_render_distance = 3;

	// Number of chunks
	int xMapChunks = 10;
	int yMapChunks = 10;

	// size of one chunk
	int chunkWidth = 256;
	int chunkHeight = 256;
	int gridPosX = 0;
	int gridPosY = 0;


	// Noise params 
	int octaves = 5;
	// height can be changed in the runtime
	float meshHeight = 64;  // Vertical scaling
	float noiseScale = 64;  // Horizontal scaling
	float persistence = 0.5;
	float lacunarity = 2;

	//Texture terrainTex;
	std::shared_ptr<Terrain_Texture> t_texture;
	std::vector<float> biomHeights;

	
	// Terrain Models 
	// spawn information of all Models of the Terrain
	std::vector<std::vector<Terrain_Model>> chunk_models;
	vector<std::shared_ptr<Model>> trees, bushes, stones;



	glm::mat4 model_to_world;
	GLuint material_id;
	// TODO should I release the Data space wenn the programm is closed or not used?
	// Terrain Mesh
	std::vector<std::vector<Vertex>> vertices_per_shape;
	std::vector<std::vector<unsigned int>> indices_per_shape;
	std::vector<std::vector<GLfloat>> aabbs_per_chunck;
	// for gl Error checking
	glErrorChecker glErrorChecker_ins;
};
