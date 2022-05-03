#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() :  Light(), 
                                        shadow_map(std::make_shared<CascadedShadowMap>()),
                                        cascade_light_matrices(NUM_CASCADES, glm::mat4(0.f))
{
    //shadow_map = new CascadedShadowMap{};
	direction = glm::vec3{0, 0, 0};
	light_proj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.f);

    
}


DirectionalLight::DirectionalLight( GLuint shadow_width, GLuint shadow_height,
                                    GLfloat red, GLfloat green, GLfloat blue,
                                    GLfloat a_intensity, GLfloat d_intensity,
                                    GLfloat x_dir, GLfloat y_dir, GLfloat z_dir,
                                    GLfloat near_plane, GLfloat far_plane,
                                    int num_cascades) : 
                                                        Light(shadow_width, shadow_height,
                                                        red, green, blue,
                                                        a_intensity, d_intensity),
                                                        shadow_map(std::make_shared<CascadedShadowMap>()),
                                                        cascade_light_matrices(NUM_CASCADES, glm::mat4(0.f))
    {

    direction = glm::vec3{ x_dir, y_dir, z_dir };
	light_proj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.f);

	shadow_map->init(shadow_width, shadow_height, num_cascades);

    shadow_near_plane = near_plane;
    shadow_far_plane = far_plane;
}

glm::mat4 DirectionalLight::get_light_view_matrix() {
	return glm::lookAt(direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

std::vector<GLfloat> DirectionalLight::get_cascaded_slots()
{
    std::vector<GLfloat> result;

    for (int i = 0; i < NUM_CASCADES + 1; i++) {

        result.push_back(cascade_slots[i]);

    }

    return result;
}

std::vector<glm::mat4>& DirectionalLight::get_cascaded_light_matrices()
{
    return cascade_light_matrices;
}

void DirectionalLight::update_shadow_map(GLfloat shadow_width, GLfloat shadow_height, GLuint num_cascades)
{
    shadow_map.reset(new CascadedShadowMap);
    shadow_map->init((GLuint)shadow_width, (GLuint)shadow_height, num_cascades);
}

void DirectionalLight::calc_cascaded_slots()
{
    GLuint number_of_elements = shadow_map->get_num_active_cascades();// sizeof(cascade_slots) / sizeof(cascade_slots[0]);

    for (int i = 0; i < NUM_CASCADES + 1; i++) {

        cascade_slots[i] = 100000.f;

    }

    for (int i = 0; i < static_cast<int>(number_of_elements + 1); i++) {
        if (i == 0) {

            (cascade_slots)[i] = shadow_near_plane;

        } else {

            (cascade_slots)[i] = (shadow_far_plane) * ((GLfloat)i / (GLfloat)(number_of_elements));

        }
    }
    return;
}

void DirectionalLight::calc_orthogonal_projections(glm::mat4 camera_view_matrix, GLfloat window_width, GLfloat window_height,
                                                   GLfloat fov, GLuint current_num_cascades)
{
    //calc the start and end point for our cascaded shadow maps
    calc_cascaded_slots();
    float aspect_ratio =  window_width / window_height;
    glm::mat4 inverse_view_matrix = glm::inverse(camera_view_matrix);
    glm::mat4 light_view_matrix = get_light_view_matrix();

    GLfloat tan_half_v_fov = glm::tan(glm::radians(((fov + 20.f) * aspect_ratio) / 2.0f));
    GLfloat tan_half_h_fov = glm::tan(glm::radians((fov + 20.f) / 2.0f));

    for (int i = 0; i < static_cast<int>(current_num_cascades); i++) {

        GLfloat xn = cascade_slots[i] * tan_half_h_fov;
        GLfloat xf = cascade_slots[i + 1] * tan_half_h_fov;
        GLfloat yn = cascade_slots[i ] * tan_half_v_fov;
        GLfloat yf = cascade_slots[i + 1] * tan_half_v_fov;

        // the # of frustum corners = 8
        glm::vec4 frustum_corners_view_space[8] = {
            //near face 
            glm::vec4(xn, yn, -cascade_slots[i], 1.0f),
            glm::vec4(-xn, yn, -cascade_slots[i], 1.0f),
            glm::vec4(xn, -yn, -cascade_slots[i], 1.0f),
            glm::vec4(-xn, -yn,-cascade_slots[i], 1.0f),
            //far face
            glm::vec4(xf, yf, -cascade_slots[i + 1], 1.0f),
            glm::vec4(-xf, yf,-cascade_slots[i + 1], 1.0f),
            glm::vec4(xf, -yf, -cascade_slots[i + 1], 1.0f),
            glm::vec4(-xf, -yf, -cascade_slots[i + 1], 1.0f)
        };

        // the # of frustum corners = 8
        glm::vec4 frustum_corners_light_space[8];
        GLfloat minX = std::numeric_limits<float>::max();
        GLfloat maxX = std::numeric_limits<float>::min();
        GLfloat minY = std::numeric_limits<float>::max();
        GLfloat maxY = std::numeric_limits<float>::min();
        GLfloat minZ = std::numeric_limits<float>::max();
        GLfloat maxZ = std::numeric_limits<float>::min();

        for (unsigned int m = 0; m < 8; m++) {
            //transform each corner from view to world space
            glm::vec4 v_world = inverse_view_matrix * frustum_corners_view_space[m];
            //now go to light space 
            frustum_corners_light_space[m] = light_view_matrix * v_world;

            minX = std::min(minX, frustum_corners_light_space[m].x);
            maxX = std::max(maxX, frustum_corners_light_space[m].x);
            minY = std::min(minY, frustum_corners_light_space[m].y);
            maxY = std::max(maxY, frustum_corners_light_space[m].y);
            minZ = std::min(minZ, frustum_corners_light_space[m].z);
            maxZ = std::max(maxZ, frustum_corners_light_space[m].z);
        }

        cascade_light_matrices[i] = glm::ortho(minX, maxX, minY, maxY, maxZ, minZ);

    }
}

glm::mat4 DirectionalLight::calculate_light_transform()
{
	return light_proj * get_light_view_matrix();
}

glm::vec3 DirectionalLight::get_direction()
{
	return direction;
}

glm::vec3 DirectionalLight::get_color()
{
	return color;
}

float DirectionalLight::get_diffuse_intensity()
{
	return diffuse_intensity;
}

float DirectionalLight::get_ambient_intensity()
{
	return ambient_intensity;
}

void DirectionalLight::set_direction(glm::vec3 direction)
{
	this->direction = direction;
}

void DirectionalLight::set_ambient_intensity(float ambient_intensity)
{
	this->ambient_intensity = ambient_intensity;
}

void DirectionalLight::set_diffuse_intensity(float diffuse_intensity)
{
	this->diffuse_intensity = diffuse_intensity;
}

void DirectionalLight::set_color(glm::vec3 color)
{
	this->color = color;
}

DirectionalLight::~DirectionalLight()
{

}