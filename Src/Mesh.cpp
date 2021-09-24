#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Mesh.h"
#include "Texture.h"


Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {

	glErrorChecker_ins.arePreError("From Mesh konstructor function in Mesh.cpp");

	this->vertices = vertices;
	this->indices = indices;
		 
	unsigned int numVertices = (int)vertices.size();
	unsigned int num_indices = (int)indices.size();

	m_drawCount = num_indices; 
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	// Dynamic Draw = lower Performence.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices[0]) * num_indices, &(this->indices[0]), GL_DYNAMIC_DRAW);

	glGenBuffers(NUM_BUFFERS, m_vab);
	glBindBuffer(GL_ARRAY_BUFFER , m_vab[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(this->vertices[0]), &(this->vertices[0]), GL_DYNAMIC_DRAW);

	//enable Vertex Atrribs for Pos, Norm, Textcood
	// Vertex Position
	//Stride bytes: just the size of Vertex, offset = use offsetof funktion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	//Vertex Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	// Vertex Texture Cood
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex,texture_coords)));
	glEnableVertexAttribArray(2);

	//unbind everything after setting the attribs
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glErrorChecker_ins.areErrorPrintAll("From Mesh konstructor function in Mesh.cpp");

}

//void Mesh::expand(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
//	// TODO?
//}


void Mesh::render() {

	glErrorChecker_ins.arePreError("From render function in Mesh.cpp");

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	//Draw Triangles
	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);

	//unbind all again
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glErrorChecker_ins.areErrorPrintAll("From render function in Mesh.cpp");

}

// Only one vertex is changed
void Mesh::changeVertex(std::vector<Vertex> vertices) {

	glErrorChecker_ins.arePreError("From changeVertex function in Mesh.cpp");

	unsigned int numVertices = (int)vertices.size();
	if (this->vertices.size() >= vertices.size()) {
		glBindBuffer(GL_ARRAY_BUFFER, m_vab[POSITION_VB]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * sizeof(this->vertices[0]), vertices.data());
	}
	// update vertices
	for (int i = 0; i < static_cast<int>(vertices.size()); i++)
	{
		this->vertices[i] = vertices[i];
	}

	glErrorChecker_ins.areErrorPrintAll("From render function in Mesh.cpp");
}



glm::mat4 Mesh::transform_Mesh(glm::vec3 translate_vec, glm::vec3 scale, float angle, glm::vec3 rotateAxis) {
	glm::mat4 transMatr = glm::mat4(1.0f);
	transMatr = glm::translate(transMatr, translate_vec);
	transMatr = glm::scale(transMatr, scale);
	transMatr = glm::rotate(transMatr, angle, rotateAxis);

	for (int i = 0; i < static_cast<int>(this->vertices.size()); i++) {

		glm::vec4 nPos = glm::vec4(this->vertices[i].position, 1.0f);
		nPos = transMatr * nPos;
		// convert vec4 to vec3. Last entry deletet? wxyz 
		this->vertices[i].position = glm::vec3(nPos);

		if (this->vertices[i].position.x != nPos.x || this->vertices[i].position.y != nPos.y || this->vertices[i].position.z != nPos.z) {
			cout << "Error from transform_Mesh in Mesh.cpp.\nConvert Vec4 to vec3 failed" << endl;
			exit(EXIT_FAILURE);
		}
	}
	changeVertex(vertices);
	return transMatr;
}


Mesh::~Mesh() {

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_ibo);
	glDeleteBuffers(NUM_BUFFERS, m_vab);

}