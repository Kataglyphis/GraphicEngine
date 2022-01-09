#pragma once
#include "ShaderProgram.h"
class ComputeShaderProgram :
    public ShaderProgram
{
public:

    ComputeShaderProgram();

    void set_noise(GLuint loc);

    GLuint get_cell_location(GLuint index);
    GLuint get_num_cell_location(GLuint index);

    void retrieve_uniform_locations();
    void reload();

    ~ComputeShaderProgram();

private:

    GLuint uniform_noise_image_location;
    GLuint uniform_cell_locations[NUM_CELLS];
    GLuint uniform_num_cell_locations[NUM_CELLS];

};

