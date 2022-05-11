#pragma once
#include "ShaderProgram.h"
#include "host_device_shared.h"

#include <GL/glew.h>

class ComputeShaderProgram :
    public ShaderProgram
{
public:

    ComputeShaderProgram();

    GLuint get_cell_location(GLuint index);
    GLuint get_num_cell_location(GLuint index);
    GLuint get_noise_image_location();

    void retrieve_uniform_locations();
    void reload();

    ~ComputeShaderProgram();

private:

    GLuint uniform_noise_image_location;
    GLuint uniform_cell_locations[NUM_CELL_POSITIONS];
    GLuint uniform_num_cell_locations[NUM_CELL_POSITIONS];

};

