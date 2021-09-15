#pragma once
#include "ShaderProgram.h"
class ShadowMapShaderProgram :
    public ShaderProgram
{
public:

    ShadowMapShaderProgram();

    GLuint get_model_location();
    GLuint get_directional_light_transform_location();
    void set_directional_light_transform(glm::mat4& l_traf);

    ~ShadowMapShaderProgram();

private:

    GLuint uniform_model_location, uniform_directional_light_transform_location;

    void retrieve_uniform_locations();
};

