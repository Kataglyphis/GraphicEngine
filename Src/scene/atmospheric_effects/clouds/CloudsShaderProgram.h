#pragma once
#include "ShaderProgram.h"

class CloudsShaderProgram :
    public ShaderProgram
{
public:

    CloudsShaderProgram();
    CloudsShaderProgram(const CloudsShaderProgram&) = default;

    GLuint get_projection_location();
    GLuint get_view_location();
    GLuint get_model_location();

    ~CloudsShaderProgram();

protected: 

    GLuint  uniform_model_location,
            uniform_view_location, 
            uniform_projection_location;

    void retrieve_uniform_locations();

};

