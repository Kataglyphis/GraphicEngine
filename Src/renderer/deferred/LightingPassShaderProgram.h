#pragma once
#include "ShaderProgram.h"
#include "PointLight.h"
#include "host_device_shared.h"

#include "UniformHelper.h"

#include "ObjMaterialLocations.h"

class LightingPassShaderProgram :
    public ShaderProgram
{
public:

	LightingPassShaderProgram();

	//all the getter for the light locations
	GLuint get_directional_light_radiance_location();
	GLuint get_directional_light_color_location();
	GLuint get_directional_light_direction_location();
	GLuint get_directional_light_shadow_intensity_location();
	GLuint get_g_position_location();
	GLuint get_g_normal_location();
	GLuint get_g_albedo_location();
	GLuint get_g_directional_light_position_location(GLuint index);
	GLuint get_directional_shadow_map_location();
	GLuint get_eye_position_location();
	GLuint get_cascade_endpoint_location(GLuint index);
	GLuint get_uniform_omni_dir_shadow_map_location(GLuint index);
	GLuint get_uniform_point_light_far_plane_location(GLuint index);
	ObjMaterialLocations get_uniform_material_locations(GLuint index);
	GLuint get_uniform_material_id_location();
	GLuint get_uniform_cloud_rad_location();
	GLuint get_uniform_cloud_offset();
	GLuint get_uniform_cloud_model();
	GLuint get_uniform_cloud_scale_location();
	GLuint get_uniform_cloud_threshold_location();
	GLuint get_directional_light_transform_location(GLuint index);
	GLuint get_g_clouds_location();
	GLuint get_random_number_location();
	GLuint get_uniform_pillowness_location();
	GLuint get_cirrus_effect_location();
	GLuint get_cloud_powderness_effect();
	GLuint get_uniform_num_active_cascades_location();
	GLuint get_uniform_pcf_radius_location();
	GLuint get_uniform_view_location();
	GLuint get_uniform_projection_location();
	GLuint get_light_matrics_id_location();

	void set_point_lights(std::vector<std::shared_ptr<PointLight>>& p_light, unsigned int texture_unit);
	void set_noise_textures(GLuint start);
	void set_cloud_texture(GLuint index);

	~LightingPassShaderProgram();

private:

	void retrieve_uniform_locations();

	GLuint uniform_light_matrics_id;

	GLuint	uniform_g_postion_location,
			uniform_g_normal_position, uniform_g_tex_color_location,
			uniform_eye_position_location,
			uniform_g_clouds_location, uniform_g_material_id_location;

	GLuint uniform_view_location, uniform_projection_location;

	GLuint uniform_directional_shadow_map_locations;
	GLuint uniform_g_directional_light_position_locations[NUM_CASCADES];

	GLuint uniform_cascade_endpoints_locations[NUM_CASCADES];

	DirectionalLightUniformLocations d_light_uniform_locations;

	GLuint uniform_point_light_count;

	struct {

		GLuint uniform_color;
		GLuint uniform_radiance;

		GLuint uniform_position;
		GLuint uniform_constant;
		GLuint uniform_linear;
		GLuint uniform_exponent;

	} uniform_point_light[MAX_POINT_LIGHTS];

	struct {

		GLuint uniform_shadow_map;
		GLuint uniform_far_plane;

	} uniform_omni_shadow_map[MAX_POINT_LIGHTS];

	ObjMaterialLocations uniform_material_locations[MAX_MATERIALS];

	GLuint uniform_noise_texture_1_location, uniform_noise_texture_2_location;

	struct uniform_cloud {

		GLuint uniform_cloud_rad_location,
		uniform_cloud_offset_location, uniform_model_to_world,
		uniform_scale_location, uniform_threshold_location, 
		uniform_pillowness_location, uniform_cirrus_effect_location, 
		uniform_powder_effect_location;

	};

	uniform_cloud cloud;

	GLuint uniform_cloud_texture_location;

	GLuint uniform_directional_light_transform_locations[NUM_CASCADES];

	GLuint uniform_random_number_location;

	GLuint uniform_num_active_cascades, uniform_pcf_radius_location;

};

