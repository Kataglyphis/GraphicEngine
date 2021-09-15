#pragma once
#include <glm/glm.hpp>;
#include <glm/gtx/hash.hpp>

struct Vertex
{
public:

	// this is public because to access the size of pos, norm, tex Cood.
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture_coords;

	Vertex() {
		this->position = glm::vec3(0);
		this->normal= glm::vec3(0);
		this->texture_coords= glm::vec2(0);


	}

	Vertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 texture_coords) { 
		this->position = pos;
		this->normal = normal;
		this->texture_coords = texture_coords;
		
	};

	glm::vec3 get_position() {
		return position;
	}

	glm::vec3 get_normal() {
		return normal;
	}

	glm::vec2 get_tex_coors() {
		return texture_coords;
	}
	
	bool operator==(const Vertex& other) const {

		return position == other.position
			&& normal == other.normal
			&& texture_coords == other.texture_coords;

	}

};

namespace std {

	template<> struct hash<Vertex> {

		size_t operator()(Vertex const& vert) const {

			size_t h1 = hash<glm::vec3>()(vert.position);
			size_t h2 = hash<glm::vec3>()(vert.normal);
			size_t h3 = hash<glm::vec2>()(vert.texture_coords);

			// combine hashed wonderfully :))))
			return ( (h1 ^ (h2  << 1)) >> 1) ^ h3;
		}
	};
}
