#pragma once
#include "ShaderProgram.h"

class OmniDirShadowShaderProgram :
    public ShaderProgram
{

public:

    OmniDirShadowShaderProgram();

    GLuint get_model_location();
    GLuint get_omni_light_pos_location();
    GLuint get_far_plane_location();
    void reload();

    void set_light_matrices(std::vector<glm::mat4> light_matrices);

    ~OmniDirShadowShaderProgram();

private: 

    GLuint uniform_model_location, uniform_omni_light_pos_location,
        uniform_far_plane_location;

    GLuint uniform_light_matrices_locations[6];

    void retrieve_uniform_locations();


};

