#pragma once
#include "Light.h"
#include <vector>
#include "OmniDirShadowMap.h"

class PointLight :
    public Light
{

public:
    PointLight();
    PointLight(GLfloat shadow_width, GLfloat shadow_height,
        GLfloat near, GLfloat far,
        GLfloat red, GLfloat green, GLfloat blue,
        GLfloat a_intensity, GLfloat d_intensity,
        GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
        GLfloat con, GLfloat lin, GLfloat exp);

    void use_light(GLuint ambient_intensity_location, GLuint ambient_color_location,
        GLuint diffuse_intensity_location, GLuint position_location,
        GLuint constant_location, GLuint linear_location, GLuint exponent_location);

    std::vector<glm::mat4> calculate_light_transform();

    void set_position(glm::vec3 position);

    OmniDirShadowMap* get_omni_shadow_map()  { return omni_dir_shadow_map; };

    GLfloat get_far_plane();
    glm::vec3 get_position();

    ~PointLight();

protected:

    OmniDirShadowMap* omni_dir_shadow_map;

    glm::vec3 position;

    GLfloat constant, linear, exponent;

    GLfloat far_plane;

};

