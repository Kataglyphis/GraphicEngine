#pragma once
#include "ShaderProgram.h"
class ShadowMapShaderProgram :
    public ShaderProgram
{
public:

    ShadowMapShaderProgram();

    GLuint get_model_location();
    GLuint get_light_matrics_id_location();
    GLuint get_VP_location();

    ~ShadowMapShaderProgram();

private:

    GLuint uniform_model_location, uniform_light_matrics_id;

    GLuint uniform_VP;

    void retrieve_uniform_locations();
};

