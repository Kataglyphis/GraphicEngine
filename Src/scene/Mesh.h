#pragma once
#include <glm/glm.hpp>
#include "Texture.h"
#include "Vertex.h"
#include <vector>
#include <glad/glad.h>
#include "GlobalValues.h"

#include "DebugApp.h"


using namespace std;

// this a simple Mesh without mesh generation
class Mesh {
public:

    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
    
    Mesh();

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
        COLOR = 2,
        TEXTURECOORD = 3
        
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
    DebugApp DebugApp_ins;

};



