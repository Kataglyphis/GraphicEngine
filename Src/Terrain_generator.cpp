#include <string>
#include <iostream>
#include <array>

#include <numeric>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Terrain_generator.h"
#include "Perlin.h"
#include "Terrain_Model.h"
#include "Terrain_texture.h"

Terrain_Generator::Terrain_Generator() {

}

void Terrain_Generator::init() {

	// initialize t_texture
	t_texture = std::make_shared<Terrain_Texture>();
	t_texture->load_all_texture();

	// Maximal Texture heights
	biomHeights = {		/*WATER_HEIGHT * 0.5f **/ -0.05f, // don`t use this tex
						WATER_HEIGHT,
						0.15,
						0.30, 
						0.4,
						0.55, 
						0.8, 
					    1.1};

	t_texture->setHeights(biomHeights, meshHeight);
	std::vector<Terrain_Model> d;
	chunk_models.resize(xMapChunks * yMapChunks, d);

	// load plants
	// load flower with tiny object loader
	//flower1 = Model();
	//tree = Model();

	set_material_id(1);

	glm::mat4 terrain_model_to_world = glm::mat4(1.f);
	terrain_model_to_world = glm::translate(terrain_model_to_world, glm::vec3(-1000.0f,0.0f,-1000.0f));

	model_to_world = terrain_model_to_world;

}

void Terrain_Generator::retreive_uniform_locations(std::shared_ptr<GeometryPassShaderProgram> shader_program)
{

	t_texture->retreive_uniform_locations(shader_program);

}


Terrain_Generator::~Terrain_Generator() {
	// destroy everything!

}

void Terrain_Generator::set_world_trafo(glm::mat4 model_to_world)
{
	this->model_to_world = model_to_world;
}

glm::mat4 Terrain_Generator::get_world_trafo()
{
	return model_to_world;
}

glm::mat4 Terrain_Generator::get_normal_world_trafo()
{
	return glm::transpose(glm::inverse(model_to_world));
}

void Terrain_Generator::set_material_id(GLuint material_id)
{
	this->material_id = material_id;
}

GLuint Terrain_Generator::get_material_id()
{
	return material_id;
}

std::shared_ptr<Terrain_Texture> Terrain_Generator::get_textures()
{
	return t_texture;
}


//Initialise the Chunk map and generate it
void Terrain_Generator::generateMesh(GLfloat& progress) {

	//terrainTex = Texture(_strdup(oneTexture), new RepeatMode());
	//if (!terrainTex.load_texture_without_alpha_channel()) {
	//	exit(EXIT_FAILURE);
	//}

	chunk_models.resize(xMapChunks * yMapChunks);


	// generate chunk map. TODO performance optimizing?
	for (int y = 0; y < yMapChunks; y++)
		for (int x = 0; x < xMapChunks; x++) {

			progress +=  (0.3f) / (GLfloat(yMapChunks) * GLfloat(xMapChunks));
			// generate everyting for one chunk
			//generate_map_chunk(x, y);
			
			generate_map_chunk(x, y, chunk_models); // later with plants
			
		}
}

void Terrain_Generator::load_plants()
{
	vector<string> treePaths = { "Models/natureModels/Tree1.1.obj",
										"Models/natureModels/Tree2.1.obj",
										"Models/natureModels/Tree2.3.obj",
										"Models/natureModels/Tree3.2.obj", // pine tree
										"Models/natureModels/Tree3.3.obj", // pine tree

	};

	vector<string> bushPaths = { "Models/natureModels/Bush1.3.obj",
									"Models/natureModels/Bush3.3.obj",

	};

	vector<string> stonePahts = {
									"Models/natureModels/Stone1.1.obj",
									"Models/natureModels/Stone1.3.obj",
									"Models/natureModels/Stone2.2.obj",
									"Models/natureModels/Stone3.2.obj",
									"Models/natureModels/Stone1.1.Lava.obj",
	};

	trees.clear();
	stones.clear();
	bushes.clear();

	for (int i = 0; i < treePaths.size(); i++)
	{

		trees.push_back(std::make_shared<Model>());
		trees[i]->load_model_in_ram(treePaths[i]);

		// Transform ??
		//glm::vec3 def0 = glm::vec3(0.0f);
		////glm::vec3 tscale = glm::vec3(0.7f);
		//glm::vec3 tscale = glm::vec3(2.7f);
		//tree.transform_model(def0, tscale);
	}

	for (int i = 0; i < bushPaths.size(); i++)
	{
		bushes.push_back(std::make_shared<Model>());
		bushes[i]->load_model_in_ram(bushPaths[i]);

		// Transform ??
		//glm::vec3 def0 = glm::vec3(0.0f);
		////glm::vec3 tscale = glm::vec3(0.7f);
		//glm::vec3 tscale = glm::vec3(2.7f);
		//tree.transform_model(def0, tscale);
	}

	for (int i = 0; i < stonePahts.size(); i++)
	{
		stones.push_back(std::make_shared<Model>());
		stones[i]->load_model_in_ram(stonePahts[i]);

		// Transform ??
		//glm::vec3 def0 = glm::vec3(0.0f);
		////glm::vec3 tscale = glm::vec3(0.7f);
		//glm::vec3 tscale = glm::vec3(2.7f);
		//tree.transform_model(def0, tscale);
	}



	//// Force that the secound texture are rgba
	//vector<bool> alphaFlags = { false, true };
	//tree.load_model("Models/tree1/Tree.obj", alphaFlags);
	//glm::vec3 def0 = glm::vec3(0.0f);
	////glm::vec3 tscale = glm::vec3(0.7f);
	//glm::vec3 tscale = glm::vec3(2.7f);
	//tree.transform_model(def0, tscale);


	//flower1.load_model("Models/rose/12974_crocus_flower_v1_l3.obj");
	//glm::vec3 fscale = glm::vec3(0.05f);
	//float newAngle = -140.0f;
	//// Rotate flower
	//flower1.transform_model(glm::vec3(0.0f), fscale, newAngle, glm::vec3(1, 0, 0));
	//// 	   Tree downsrender
	////tree.load_model("Models/test/10447_Pine_Tree_v1_L3b.obj");

}


// generate_map_chunk
// this function generate and assiged the Chunk data
// x and y offsets are the chunk position
void Terrain_Generator::generate_map_chunk(int xOffset, int yOffset , std::vector< std::vector<Terrain_Model>>& chunk_models) {

	GLfloat minX = std::numeric_limits<float>::max();
	GLfloat maxX = std::numeric_limits<float>::min();
	GLfloat minY = std::numeric_limits<float>::max();
	GLfloat maxY = std::numeric_limits<float>::min();
	GLfloat minZ = std::numeric_limits<float>::max();
	GLfloat maxZ = std::numeric_limits<float>::min();

	std::vector<GLuint> indices;
	std::vector<float> noise_map;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> smooth_normals;
	//std::vector<float> vertices;
	//std::vector<float> normals;

	//std::vector<Terrain_Model> plants;
	std::vector<glm::vec2> texCoords;

	// Generate map

	std::vector<Vertex> mesh_vertex;
	indices = generate_indices(); // generate indices for the Triangle mesh
	noise_map = generate_noise_map(xOffset, yOffset); // returns noise Map based on the chunk offsets (only normalized noise Hight)
	vertices = generate_vertices(noise_map); // return 127*127 vertices {vec3(x0, noise0, y0), vec3(x1, noise1, y1), vec3(x2, noise2, y2),...}
	normals = generate_normals(indices, vertices); // return 126*126*2 normals = one normal for one triangle
	// make smooth normals, because vertex has multiple normals -> smooth normals
	smooth_normals = calc_smooth_normals(vertices, normals, indices);

	
	// generate Texter coord
	texCoords = generate_biome(vertices, indices, xOffset, yOffset, chunk_models);


	// Size should be:
	// size indices 126*126*2*3 entries
	// => here Triangle 126 * 126 * 2 (ChunkHeight -1) * (Chunkwidth-1) * 2 Triangles
	int sizeIndices = indices.size();
	// -> return a size of a normal = #vertices here 127*127
	int sizeVertices = vertices.size();

	mesh_vertex.clear();

	// savty check
	if (smooth_normals.size() != sizeVertices || sizeVertices != texCoords.size()) {
		cout << "Error, from generate_map_chunk form Terrain_Generator class: Size of the vertices and size of the normals doesnt match." << endl;
		exit(EXIT_FAILURE);
	}

	// assgin the data to a Vertex Vector as a Chunk Vertex 
	for (int i = 0; i < vertices.size(); i++)
	{
		glm::vec3 offset = glm::vec3(xOffset * (chunkWidth -1) , 0, yOffset * (chunkHeight - 1));
		mesh_vertex.push_back(Vertex(vertices[i] + offset, smooth_normals[i], texCoords[i]));

		glm::vec3 pos = vertices[i] + offset;
		minX = std::min(minX, pos.x);
		maxX = std::max(maxX, pos.x);
		minY = std::min(minY, pos.y);
		maxY = std::max(maxY, pos.y + 100); // maybe  100 because of plants
		minZ = std::min(minZ, pos.z);
		maxZ = std::max(maxZ, pos.z);
	}

	/*AABB* map_chunck_aabb = new AABB();
	map_chunck_aabb->init(minX, maxX, minY, maxY, minZ, maxZ);*/

	vertices_per_shape.push_back(mesh_vertex);
	indices_per_shape.push_back(indices);
	std::vector<GLfloat> current_aabb = { minX, maxX, minY, maxY, minZ, maxZ };
	aabbs_per_chunck.push_back(current_aabb);
	/*Mesh* obj_1 = new Mesh(mesh_vertex, indices);
	meshes.push_back(obj_1);
	aabbs.push_back(map_chunck_aabb);*/

}

void Terrain_Generator::generate_render_context()
{
	for (int i = 0; i < vertices_per_shape.size(); i++) {

		this->meshes.push_back(std::make_shared<Mesh>(vertices_per_shape[i], indices_per_shape[i]));
		std::shared_ptr<AABB> map_chunck_aabb = std::make_shared<AABB>();

		map_chunck_aabb->init(aabbs_per_chunck[i][0], 
													aabbs_per_chunck[i][1],
													aabbs_per_chunck[i][2],
													aabbs_per_chunck[i][3],
													aabbs_per_chunck[i][4],
													aabbs_per_chunck[i][5]	
													); // minX, maxX, minY, maxY, minZ, maxZ

		aabbs.push_back(map_chunck_aabb);

	}

	for (std::shared_ptr<Model> tree : trees) {
		tree->create_render_context();
	}

	for (std::shared_ptr<Model> bush : bushes) {
		bush->create_render_context();
	}

	for (std::shared_ptr<Model> stone : stones) {
		stone->create_render_context();
	}

}

//void Terrain_Generator::setup_instancing(GLuint& VAO, std::vector<GLuint>& plant_chunk, std::string plant_type, std::vector<Plant>& plants, std::string filename) {
//}

// returns normalized NoiseValues (noise_map is size of chunkHeight*chunkWidth)
// offsetx and offsety are the chunk offsets (offsetx, offsety). botten left junk are (0,0), top right chunk are (10, 10)
std::vector<float> Terrain_Generator::generate_noise_map(int offsetX, int offsetY) {
	std::vector<float> noiseValues;
	std::vector<float> normalizedNoiseValues;
	std::vector<int> p = getPermutationVector();


	float amp = 1;
	float freq = 1;
	float maxPossibleHeight = 0;

	for (int i = 0; i < octaves; i++) {
		maxPossibleHeight += amp;
		amp *= persistence;
	}

	for (int y = 0; y < chunkHeight; y++) {
		for (int x = 0; x < chunkWidth; x++) {
			// iterate over the chunk


			// todo Change these Parameter
			// change these amplitute hill hight
			amp = 1;
			// hill frequenzy
			freq = 1;

			float noiseHeight = 0;

			for (int i = 0; i < octaves; i++) {
				float xSample = (x + offsetX * (chunkWidth - 1)) / noiseScale * freq;
				float ySample = (y + offsetY * (chunkHeight - 1)) / noiseScale * freq;

				// set noise hight for the chunk (y coordinate)
				float perlinValue = perlin_noise(xSample, ySample, p);
				noiseHeight += perlinValue * amp;

				// Lacunarity  --> Increase in frequency of octaves
				// Persistence --> Decrease in amplitude of octaves
				amp *= persistence;
				freq *= lacunarity;
			}

			noiseValues.push_back(noiseHeight);
		}
	}



	for (int y = 0; y < chunkHeight; y++) {
		for (int x = 0; x < chunkWidth; x++) {
			// Inverse lerp and scale values to range from 0 to 1
			normalizedNoiseValues.push_back((noiseValues[x + y * chunkWidth] + 1) / maxPossibleHeight);
		}
	}

	return normalizedNoiseValues;
}


// -> generate position here for vertex (coordinate , norm, texture coordinate)
/// return vertices = {x0, noise0, y0, x1, noise1, y1, x2, noise2, y2, ....}
// (chunk-width, noise heigt, chunk-height)
std::vector<glm::vec3> Terrain_Generator::generate_vertices(const std::vector<float>& noise_map) {

	// generate position and save these in vector v
	std::vector<glm::vec3> v;

	// Check if size is big enough
	if (noise_map.size() < (chunkHeight * chunkHeight)) {
		cout << "From Terrain_Generator::generate_vertices-> Error, size of the noise_map is to small" << endl;
		exit(EXIT_FAILURE);
	}

	// why chunkHeight +1?? because 
	for (int y = 0; y < chunkHeight; y++)
		for (int x = 0; x < chunkWidth; x++) {

			// Apply cubic easing to the noise
			// Scale noise to match meshHeight // changed 1.1 to 1.0
			float easedNoise = std::pow(noise_map[x + y * chunkWidth] * 1.0, 3);
			// Pervent vertex height from being below WATER_HEIGHT
			float hight = std::fmax(easedNoise * meshHeight, WATER_HEIGHT * 0.5 * meshHeight);

			v.push_back(glm::vec3((float)x, hight, (float)y));
		}

	return v;
}


// return chunkHeight-1 * chunkHeight-1 * 2 Triangles = here 16256*2 = 31752 Triangles
std::vector<GLuint> Terrain_Generator::generate_indices() {
	std::vector<GLuint> indices;

	for (int y = 0; y < chunkHeight; y++)
		for (int x = 0; x < chunkWidth; x++) {
			// iterate chunkheight * chunkwidth = here 16256
			int pos = x + y * chunkWidth;

			if (x == chunkWidth - 1 || y == chunkHeight - 1) {
				// Don't create indices for right or top edge
				continue;
			}
			else {
				// Bottem left triangle of square
				indices.push_back(pos); // Bottem left
				indices.push_back(pos + chunkWidth); // top left
				indices.push_back(pos + 1); // bottem right
				// Top right triangle of square
				indices.push_back(pos + 1); // bottem right
				indices.push_back(pos + chunkWidth); // top left
				indices.push_back(pos + chunkWidth + 1); // top right
			}
		}

	return indices;
}


// Calculate the normal of the Triangles vertex.
// Return: normals with size of Trianlge, that contains in indices. here 126*126*2 = 31752
std::vector<glm::vec3> Terrain_Generator::generate_normals(const std::vector<GLuint>& indices, const std::vector<glm::vec3>& vertices) {
	int pos;
	glm::vec3 normal;
	std::vector<glm::vec3> normals; // changed float into glm::vec3
	std::vector<glm::vec3> verts; // changed float vector to glm::vec3

	// Get the vertices of each triangle in mesh
	// For each group of indices

	// 126*126 *2 Triangle ind
	int indSize = indices.size(); // size if 95256 (triangle(a,b,c), triangle(d,b,c), ... ) = [t1.1, t1.2, t1.3, t2.1, t2.2, t2.3, ...]
	int verSize = vertices.size(); // is chunkHight * chunkWidth = here 16129 = 11111100000001

	for (int i = 0; i < indices.size(); i += 3) {
		// iterate over all trianlge (number Triangle = 126*126*2)

		// Get the 3 vertices for each trianlge
		for (int j = 0; j < 3; j++) {

			// Ignore this
			//pos = indices[i + j] * 3; /LiamHz Code:  *3 <- because vertices was one dimensional [x1, y1, z1, x2, y2, z3]
			// Now we have [vec3, vec3, vec3] => 3 times smaller


			// get pos of the vertex in vertices
			pos = indices[i + j];
			if (pos < vertices.size()) {
				// index is in the range of the vertice 


				// Had an error because Kansei changed the vertice vector Type from vector<float>  to vector<vec3>.
				//verts.push_back(glm::vec3(vertices[pos], vertices[pos + 1], vertices[pos + 2])); // LiamHZ Code
				verts.push_back(vertices[pos]);
			}
			else
			{
				// Error!! 
				printf("\n Error! From Terrain_generator -> generate_normals function. Vertices out of range!\n");
				exit(EXIT_FAILURE); // should it exit? 
			}
		}
		// size verts = indices.size
		// size indices = 126*126*2
		// -> return a size of a normal = #vertices
		// Get vectors of two edges of triangle
		glm::vec3 U = verts[i + 1] - verts[i];
		glm::vec3 V = verts[i + 2] - verts[i];

		// Calculate normal
		normal = glm::normalize(glm::cross(U, V));
		normals.push_back(normal);
	}

	return normals;
}


std::vector<glm::vec3> Terrain_Generator::calc_smooth_normals(std::vector<glm::vec3> verticesPos, std::vector<glm::vec3> triangleNormals, std::vector<unsigned int> indices) {
	int trianlgeIndex = 0;
	std::vector<std::vector<glm::vec3>> normalCollector;
	normalCollector.resize(verticesPos.size());
	std::vector<glm::vec3> smoothNormals;

	for (int i = 0; i < indices.size(); i += 3) {
		// iterate over all Trianlges here sizeIndices = 9526 -> 31.752 Triangles

		
		for (int j = 0; j < 3; j++)
		{
			// iterate over all Triangle Points
			int ver_index = indices[i + j];
			normalCollector[ver_index].push_back(triangleNormals[trianlgeIndex]);
		}
		trianlgeIndex += 1;
	}

	// std::for_each(normalCollector.begin(), normalCollector.end(), [](std::vector<glm::vec3> normals) {std::accumulate(normals.begin(), normals.end(), glm::vec3(0)) / (float)normals.size(); });

	for (int i = 0; i < normalCollector.size(); i++)
	{
		if (normalCollector[i].size() <=0) {
			cout << "Error from the calc_smooth_normals() function in Terrain_generator class" << endl;
			exit(EXIT_FAILURE);
		}
		smoothNormals.push_back( std::accumulate(normalCollector[i].begin(), normalCollector[i].end(), glm::vec3(0)) / (float)normalCollector[i].size());
	}

	return smoothNormals;
}


std::vector<bool> Terrain_Generator::render(GLfloat ratio, std::shared_ptr<Camera> camera, std::shared_ptr<ViewFrustumCulling> view_cull, RenderPassSceneDependend* shader_pass) {
	
	//get_textures()->use_texture();
	// this to flag with chunks is rendered
	std::vector<bool> is_Chunk_Render_Flag;
	is_Chunk_Render_Flag.resize(meshes.size(), false);



	for (int i = 0; i < meshes.size(); i++) {
		//texture_list[shapes_to_tex[i]]->use_texture();
		std::shared_ptr<AABB> aabb = aabbs[i];
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//aabb->render();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (view_cull->is_inside(ratio, camera, aabb, get_world_trafo())) {

			meshes[i]->render();
			is_Chunk_Render_Flag[i] = true;

		}
	}

	//get_textures()->unbind_texture(); <--- this needed, otherwise the is terrain is set forever
	// move this outside
	// render_plants(is_Chunk_Render_Flag, shader_pass);

	return is_Chunk_Render_Flag;
}

struct terrainType {
	terrainType(float _height, glm::vec2 _texCoord) {
		height = _height;
		texCoord = _texCoord;
	};
	float height;
	glm::vec2 texCoord;
};



// generate the texture and the plant position
std::vector<glm::vec2> Terrain_Generator::generate_biome(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices, int xOffset, int yOffset, std::vector<std::vector<Terrain_Model>> & chunk_models) {
	
		
	std::vector<glm::vec2> texCoords;

	// ----------------------simple texture coordinates:----------------------
	glm::vec2 default_vec2 = glm::vec2(-1.0f, -1.0f);
	texCoords.resize(vertices.size(), default_vec2);

	for (int y = 0; y < chunkHeight; y++)
	{
		for (int x = 0; x < chunkWidth; x++)
		{
			texCoords[x + y * chunkWidth] = glm::vec2(((float)x) / 16.0f , ((float)y) / 16.0f);
		}
	}


	// Iterate over the chunkPlants!!
	// 
	// ----------------------Define Terrain_Model spawn Position ----------------------
	 //Iterate through vertex y values
	for (int i = 0; i < vertices.size(); i += 1) {
		 /*
		 iterate over all triangles
				 go throw all triangles. calculate the average hight of the vertex. 
		 decide which the textue you need.
		 */

		for (int j = 1; j < biomHeights.size(); j++) {
			// NOTE: The max height of a vertex is ca. "meshHeight"
			// Wird garnicht erreicht......
			if (vertices[i].y <= biomHeights[j] * meshHeight || j == biomHeights.size() - 1) {
				// found a match between vertex height and 

				if (j > 3) { // light grean biom
					Terrain_Model::M_TYPE plantType;
					int variantSelection;

					if (rand() % 1000 < 3) {
						if (rand() % 60 < 30) {
							//plant pine trees in heigher bioms
							variantSelection = (int)(3 + rand() % trees.size() - 3);
							plantType = Terrain_Model::TREE;

						}
						else
						{
							//plantType = Terrain_Model::aousdha ;
							variantSelection = (int)(rand() % stones.size());
							plantType = Terrain_Model::STONE;
						}
					}



					// vertices are in space of (x,y,z) where x = [0,256] , y = [0, max Mesh], z = [0,256]
					glm::vec3 offset = glm::vec3(xOffset * (chunkWidth - 1), 0, yOffset * (chunkHeight - 1));
					glm::vec3 plant_pos = vertices[i] + offset;
					int checkNumber = chunk_models[xOffset + yOffset * xMapChunks].size();
					chunk_models[xOffset + yOffset * xMapChunks].push_back(Terrain_Model(plantType, plant_pos, xOffset, yOffset, variantSelection));
					if (chunk_models[xOffset + yOffset * xMapChunks].size() == checkNumber) {
						cout << "Error something went wrong. vector no expanded" << endl;
						exit(EXIT_FAILURE);
					}

				}
				break;
			}


			// plant Plants
			if (j == 3) { // light grean biom
				Terrain_Model::M_TYPE plantType;
				int variantSelection;
				if (rand() % 1000 < 3) {
					if (rand() % 100 < 40) {
						variantSelection = (int)(rand() % bushes.size());
						plantType = Terrain_Model::BUSH;
					}
					else {
						if (rand() % 60 < 30) {
							//plantType = Terrain_Model::aousdha ;
							variantSelection = (int)(rand() % trees.size());
							plantType = Terrain_Model::TREE;

						}
						else
						{
							//plantType = Terrain_Model::aousdha ;
							variantSelection = (int)(rand() % stones.size());
							plantType = Terrain_Model::STONE;
						}
					}
					// Optional TODO: randam place on the triangle surface via interpolation 
					/*
						0) Multiple plant per surface
						1) Interpolation a random point (lerp(a,b, randomNumber[0,1]) function) between (a, b) = ab
						2) and (a, c) = ac
						3) and (ab, ac) => random spawn punkt.
						4) (optinal) if (spawn point is too close) to the other reroll

						b
					   / \
					  /  *\
					 a_____c

					*/


					// vertices are in space of (x,y,z) where x = [0,256] , y = [0, max Mesh], z = [0,256]
					glm::vec3 offset = glm::vec3(xOffset * (chunkWidth - 1), 0, yOffset * (chunkHeight - 1));
					glm::vec3 plant_pos = vertices[i] + offset;
					int checkNumber = chunk_models[xOffset + yOffset * xMapChunks].size();
					chunk_models[xOffset + yOffset * xMapChunks].push_back(Terrain_Model(plantType, plant_pos, xOffset, yOffset, variantSelection));
					if (chunk_models[xOffset + yOffset * xMapChunks].size() == checkNumber) {
						cout << "Error something went wrong. vector no expanded" << endl;
						exit(EXIT_FAILURE);
					}

				}
				break;
			}
		}
		
	}
	

	// test if all texCoord are assigned
	std::vector<int> not_index;
	for (int i = 0; i < texCoords.size(); i++)
	{
		if (texCoords[i] == default_vec2) {

			not_index.push_back(i);
			// Print all i's 
			cout << "Error from generate_biome function. Not all TexCoord are assigned to a coordinate" << endl;
		}
	}
	if (not_index.size() >= 1) {
			exit(EXIT_FAILURE);
	}
	// savty check end


	return texCoords;
}

void Terrain_Generator::regenerate() {

	// update new terrain texture
	t_texture->setHeights(biomHeights, meshHeight);

	// TODO calculate new vertex YOU CAN DO IT BETTER because the function is: float hight = std::fmax(easedNoise * meshHeight, WATER_HEIGHT * 0.5 * meshHeight);
	for (int y = 0; y < yMapChunks; y++) {
		for (int x = 0; x < xMapChunks; x++) {

			// calc new vertices
			std::vector<GLuint> indices = meshes[x + y * xMapChunks]->getIndices();
			std::vector<Vertex> tempVertex = meshes[x + y * xMapChunks]->getVertices();
			std::vector<float> noiseVec = generate_noise_map(x, y);
			std::vector<glm::vec3> newPos = generate_vertices(noiseVec);

			// Calculate smooth normals
			std::vector<glm::vec3> newNormals = generate_normals(indices, newPos);

			 newNormals = calc_smooth_normals(newPos, newNormals, indices);


			if (newPos.size() != tempVertex.size()) {
				cout << "Error from ChangeMaxHeight, new generated Vertex size isn't equal to the als size." << endl;
				exit(EXIT_FAILURE);
			}
			for (int i = 0; i < newPos.size(); i++)
			{
				// update new Vertex positions
				glm::vec3 vertex_offset = { x * (chunkWidth-1), 0 , y * (chunkHeight -1 )};
				tempVertex[i].position = newPos[i] + vertex_offset;
				tempVertex[i].normal = newNormals[i];
				// 
			}

			// change the vertex in mesh class
			meshes[x + y]->changeVertex(tempVertex);
		}
	}
}


void Terrain_Generator::changeMaxHeight(float newMaxHeight) {
	
	if (newMaxHeight <= 0) newMaxHeight = 1;

	// calculate new vertex 
	for (int y = 0; y < yMapChunks; y++) {
		for (int x = 0; x < xMapChunks; x++) {
			
			// calc new vertices
			std::vector<Vertex> tempVertex = meshes[x + y*xMapChunks]->getVertices();

			// TODO Calculate new normals

			for (int i = 0; i < tempVertex.size(); i++)
			{
				tempVertex[i].position = glm::vec3(tempVertex[i].position.x, (tempVertex[i].position.y/meshHeight) * newMaxHeight, tempVertex[i].position.z);
			}
			
			//const auto vaddress = &newV;
			meshes[x + y* xMapChunks]->changeVertex(tempVertex);
		}

	}
	// change max mesh height
	meshHeight = newMaxHeight;
	// todo change the vertex in mesh class
}



void Terrain_Generator::render_plants(std::vector<bool> is_chunk_rendered_flags, RenderPassSceneDependend* shader_pass) {
		

		// All flowers are on the chunk (1,y), where y 0-255
		for (int i = 0; i < chunk_models.size(); i++)
		{
			int chunk_index = i;

			// check if the chunk will rendered.
			if (is_chunk_rendered_flags[chunk_index]) {

				// render all plants
				for (int model_index = 0; model_index < chunk_models[chunk_index].size(); model_index++) // 
				{
					glErrorChecker_ins.arePreError(" GL Error, from render_plants function in Terrain_generator.cpp.\nFunction Iteration: Chunk_index = " + std::to_string(chunk_index) + ", plant Index = " + std::to_string(model_index));

					glm::mat4 model_matrix_terrain = glm::mat4(1.0f);

					glm::mat4 model_matrix_terrain_offeset = get_world_trafo();

					glm::vec3 plant_pos = chunk_models[chunk_index][model_index].pos;
					model_matrix_terrain = glm::translate(model_matrix_terrain, plant_pos);
					model_matrix_terrain = model_matrix_terrain * model_matrix_terrain_offeset;
					float angle = chunk_models[chunk_index][model_index].angle;
					model_matrix_terrain = glm::rotate(model_matrix_terrain, angle, glm::vec3(0.0f, 1.0f, 0.0f));
					model_matrix_terrain = glm::scale(model_matrix_terrain, glm::vec3(10.f, 10.f, 10.f));

					glm::mat4 normal_matr = glm::transpose(glm::inverse(model_matrix_terrain));// glm::transpose(glm::inverse(model_matrix_terrain_offeset));

					shader_pass->set_game_object_uniforms(model_matrix_terrain, normal_matr, 0);
				
					switch (chunk_models[chunk_index][model_index].type)
					{
					case(Terrain_Model::TREE):
						if (chunk_models[chunk_index][model_index].variation >= trees.size()) {
							cout << "Error, out of bounds from renderPlants function in Terrain_generatr.cpp" << endl;
						}
						//trees[chunk_models[chunk_index][model_index].variation]->create_render_context();
						trees[chunk_models[chunk_index][model_index].variation]->render();
						break;
					case(Terrain_Model::BUSH):
						if (chunk_models[chunk_index][model_index].variation >= bushes.size()) {
							cout << "Error, out of bounds from renderPlants function in Terrain_generatr.cpp" << endl;
						}
						//bushes[chunk_models[chunk_index][model_index].variation]->create_render_context();
						bushes[chunk_models[chunk_index][model_index].variation]->render();
						break;
					case(Terrain_Model::STONE):
						if (chunk_models[chunk_index][model_index].variation >= stones.size()) {
							cout << "Error, out of bounds from renderPlants function in Terrain_generatr.cpp" << endl;
						}
						//stones[chunk_models[chunk_index][model_index].variation]->create_render_context();
						stones[chunk_models[chunk_index][model_index].variation]->render();
						break;
					default:
						cout << "Warning from render_plants function in Terrain_gerneration.cpp, some undefined Terrain_Model Types are set." << endl;
						break;
					}

				// Check for gl Errors
				glErrorChecker_ins.arePreError("From render_plants function in Terrain_generator.cpp.\nFunction Iteration: Chunk_index = " + std::to_string(chunk_index) + ", Model Index = " + std::to_string(model_index));
				}
			}
		}
	
}


//TODO use this if you reach the limit of the map_chunks.
void Terrain_Generator::expandTerrain() {

}


