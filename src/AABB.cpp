#include "AABB.h"

#include "glErrorChecker.h"

AABB::AABB()
{
}

std::vector<glm::vec3> AABB::get_corners(glm::mat4 model)
{
    std::vector<glm::vec3> corners_world_space;

    for (glm::vec3 corner : corners) {

        corners_world_space.push_back(glm::vec3(model * glm::vec4(corner,1.0f)));

    }

    return corners_world_space;

}



void AABB::init(GLfloat minX, GLfloat maxX, GLfloat minY, GLfloat maxY, GLfloat minZ, GLfloat maxZ)
{
    this->minX = minX;
    this->maxX = maxX;
    this->minY = minY;  
    this->maxY = maxY; 
    this->minZ = minZ;
    this->maxZ = maxZ;

     corners.push_back(glm::vec3(minX, minY, minZ));
     corners.push_back(glm::vec3(minX, minY, maxZ));
     corners.push_back(glm::vec3(minX, maxY, minZ));
     corners.push_back(glm::vec3(minX, maxY, maxZ));
     corners.push_back(glm::vec3(maxX, minY, minZ));
     corners.push_back(glm::vec3(maxX, minY, maxZ));
     corners.push_back(glm::vec3(maxX, maxY, minZ));
     corners.push_back(glm::vec3(maxX, maxY, maxZ));

	 unsigned int num_corners = (int)corners.size();
     m_drawCount = 36;//num_corners;

     float vertices[] = {

        minX, minY, minZ, // left bottom front
        minX, minY, maxZ, // left bottom back
         minX, maxY, minZ, // left top front
         minX, maxY, maxZ, // left top back
         maxX, minY, minZ, // right bottom front
         maxX, minY, maxZ, //right bottom back
         maxX, maxY, minZ, //right top front
         maxX, maxY, maxZ //right top back

     };

     unsigned int indices[] = {  // note that we start from 0!
        //left 
        0, 3, 1,  
        0, 2, 3,
        //right 
        4, 7, 5,
        4, 6, 7,
        //top
        3, 7, 2,
        2, 7, 6,
        //bottom
        0, 1, 5,
        0, 5, 4,
        //back
        1, 5, 3,
        5, 7, 3,
        //front
        0, 4, 2,
        4, 6, 2,
     };

     // check with glErrorChecker.h if they are any glError before executing gl stuffs and print it.
     if (glErrorChecker.arePreError("From init function in AABB.")) {
         // to something?
     }

     glGenVertexArrays(1, &VAO);
     glGenBuffers(1, &VBO);
     glGenBuffers(1, &EBO);
     // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
     glBindVertexArray(VAO);

     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(0);

     // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
     //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

     // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
     // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
     glBindVertexArray(0);

     // check with glErrorChecker.h if they are any glError and print it.
     if (glErrorChecker.areErrorPrintAll("From init function in AABB.cpp file.")) {
         // return false;
     }
}

void AABB::render()
{

    // check with glErrorChecker.h if they are any glError before executing gl stuffs and print it.
    if (glErrorChecker.arePreError("From render function in AABB.")) {
        // to something?
    }

    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);

    //unbind all again
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // check with glErrorChecker.h if they are any glError and print it.
    if (glErrorChecker.areErrorPrintAll("From render function in AABB.cpp file.")) {
        // return false;
    }
}

AABB::~AABB()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
