#pragma once

#include<glm/glm.hpp>


class Terrain_Model
{
public:
    enum M_TYPE
    {
        BUSH,
        TREE,
        STONE,
    };

    Terrain_Model(M_TYPE type, glm::vec3 pos, int xOffset, int yOffset, int variation = 0) {
        this -> type = type;
        this->pos = pos;
        this->xOffset = xOffset;
        this->yOffset = yOffset;
        this->variation = variation;

        this->angle = glm::radians((float)(rand() % 360));
    }


    M_TYPE type;
    glm::vec3 pos;
    int xOffset;
    int yOffset;
    int variation;
    float angle;

private:

};