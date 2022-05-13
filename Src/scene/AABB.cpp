#include "AABB.h"

#include "DebugApp.h"

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

    vertices.push_back(Vertex(glm::vec3(minX, minY, minZ), glm::vec3(0.f), glm::vec3(0.f), glm::vec2(0.f)));
    vertices.push_back(Vertex(glm::vec3(minX, minY, maxZ), glm::vec3(0.f), glm::vec3(0.f), glm::vec2(0.f)));
    vertices.push_back(Vertex(glm::vec3(minX, maxY, minZ), glm::vec3(0.f), glm::vec3(0.f), glm::vec2(0.f)));
    vertices.push_back(Vertex(glm::vec3(minX, maxY, maxZ), glm::vec3(0.f), glm::vec3(0.f), glm::vec2(0.f)));
    vertices.push_back(Vertex(glm::vec3(maxX, minY, minZ), glm::vec3(0.f), glm::vec3(0.f), glm::vec2(0.f)));
    vertices.push_back(Vertex(glm::vec3(maxX, minY, maxZ), glm::vec3(0.f), glm::vec3(0.f), glm::vec2(0.f)));
    vertices.push_back(Vertex(glm::vec3(maxX, maxY, minZ), glm::vec3(0.f), glm::vec3(0.f), glm::vec2(0.f)));
    vertices.push_back(Vertex(glm::vec3(maxX, maxY, maxZ), glm::vec3(0.f), glm::vec3(0.f), glm::vec2(0.f)));

    indices = {  // note that we start from 0!
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

    mesh = std::make_shared<Mesh>(vertices, indices);

    // check with DebugApp_ins.h if they are any glError before executing gl stuffs and print it.
    if (DebugApp_ins.arePreError("From init function in AABB.")) {
        // to something?
    }

}

void AABB::render()
{

    // check with DebugApp_ins.h if they are any glError before executing gl stuffs and print it.
    if (DebugApp_ins.arePreError("From render function in AABB.")) {
        // to something?
    }

    mesh->render();

    // check with DebugApp_ins.h if they are any glError and print it.
    if (DebugApp_ins.areErrorPrintAll("From render function in AABB.cpp file.")) {
        // return false;
    }
}

AABB::~AABB()
{
   
}
