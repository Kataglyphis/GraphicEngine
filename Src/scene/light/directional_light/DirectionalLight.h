#pragma once
#include "Light.h"
#include <memory>
#include <limits>
#include "host_device_shared.h"
#include <vector>
#include <array>

class DirectionalLight : public Light
{
public:

	DirectionalLight();

	DirectionalLight(	GLuint shadow_width, GLuint shadow_height,
						GLfloat red, GLfloat green, GLfloat blue,
						GLfloat a_intensity, GLfloat d_intensity,
						GLfloat x_dir, GLfloat y_dir, GLfloat z_dir,
						GLfloat near_plane, GLfloat far_plane, int num_cascades);

	glm::mat4 calculate_light_transform();

	std::shared_ptr<CascadedShadowMap> get_shadow_map() { return shadow_map; }

	glm::vec3 get_direction();
	glm::vec3 get_color();
	float get_diffuse_intensity();
	float get_ambient_intensity();
	glm::mat4 get_light_view_matrix();
	std::vector<GLfloat> get_cascaded_slots();
	std::vector<glm::mat4>& get_cascaded_light_matrices();

	void update_shadow_map(GLfloat shadow_width, GLfloat shadow_height, GLuint num_cascades);

	void calc_orthogonal_projections(	glm::mat4 camera_view_matrix, 
										GLfloat fov,
										GLuint window_width,
										GLuint window_height,
										GLuint current_num_cascades);

	void set_direction(glm::vec3 direction);
	void set_ambient_intensity(float ambient_intensity);
	void set_diffuse_intensity(float diffuse_intensity);
	void set_color(glm::vec3 color);

	~DirectionalLight();

private:

	std::vector<glm::vec4> get_frustum_corners_world_space(const glm::mat4& proj, const glm::mat4& view);
	void calc_cascaded_slots();

	std::shared_ptr<CascadedShadowMap> shadow_map;
	//CascadedShadowMap*  shadow_map;
	glm::vec3 direction;
	GLfloat shadow_near_plane, shadow_far_plane;

	std::array<GLfloat, NUM_CASCADES + 1>	cascade_slots;
	std::vector<glm::mat4>					cascade_light_matrices;
};

