#pragma once
#include <glm/glm.hpp>
#include "Texture.h"
#include "Vertex.h"
#include <vector>
#include <GL/glew.h>

#include "GlobalValues.h"

#include "glErrorChecker.h"


using namespace std;

// this a simple Mesh without mesh generation
class Mesh {
public:

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    
    Mesh();


    glm::mat4 transform_Mesh(glm::vec3 translate_vec, glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f, glm::vec3 rotateAxis = glm::vec3(1.0f, 0.0f, 0.0f));
    void expand(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void changeVertex(std::vector<Vertex> vertices);
    void render();
    std::vector<Vertex> getVertices() {
        return this->vertices;
    }
    std::vector<unsigned int> getIndices() {
        return this->indices;
    }

     ~Mesh();

private:
    //  render data
    // unsigned int VAO, VBO, EBO;

    enum {
        POSITION = 0,
        NORMAL = 1,
        TEXTURECOORD = 2
        
    };

    enum
    {
        POSITION_VB,
        NUM_BUFFERS
    };

    // Vertex Array Object
    GLuint m_vao, m_ibo;
    // Vertex array buffer
    GLuint m_vab[NUM_BUFFERS];

    unsigned int m_drawCount;
    std::vector<Vertex> vertices; 
    std::vector<unsigned int> indices;

    // for gl error checks
    glErrorChecker glErrorChecker_ins;

};



