#include "LightingPassShaderProgram.h"

LightingPassShaderProgram::LightingPassShaderProgram()
{
}

void LightingPassShaderProgram::retrieve_uniform_locations() {

    // check if there any errors appears before executing the function
    DebugApp_ins.arePreError("From retrieve_uniform_locations in LightingPassShaderProgram.cpp");

    //setting all uniforms for our directional lights
    d_light_uniform_locations.uniform_color_location                = glGetUniformLocation(program_id, "directional_light.base.color");
    d_light_uniform_locations.uniform_ambient_intensity_location    = glGetUniformLocation(program_id, "directional_light.base.ambient_intensity");
    d_light_uniform_locations.uniform_diffuse_intensity_location    = glGetUniformLocation(program_id, "directional_light.base.diffuse_intensity");
    d_light_uniform_locations.uniform_direction_location            = glGetUniformLocation(program_id, "directional_light.direction");
    d_light_uniform_locations.uniform_shadow_intensity_location     = glGetUniformLocation(program_id, "directional_light.shadow_intensity");

    //buffer locations
    uniform_g_postion_location      = glGetUniformLocation(program_id, "g_position");
    uniform_g_normal_position       = glGetUniformLocation(program_id, "g_normal");
    uniform_g_tex_color_location    = glGetUniformLocation(program_id, "g_albedo");
    uniform_eye_position_location   = glGetUniformLocation(program_id, "eye_position");
    uniform_g_frag_depth_location   = glGetUniformLocation(program_id, "g_frag_depth");
    uniform_g_material_id_location  = glGetUniformLocation(program_id, "g_material_id");

    for (size_t i = 0; i < NUM_CASCADES; i++) {

        char loc_buff[100] = { '\0' };
        snprintf(loc_buff, sizeof(loc_buff), "cascade_endpoints[%zd]", i);
        uniform_cascade_endpoints_locations[i] = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "g_directional_light_positions[%zd]", i);
        uniform_g_directional_light_position_locations[i] = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "directional_shadow_maps[%zd]", i);
        uniform_directional_shadow_map_locations[i] = glGetUniformLocation(program_id, loc_buff);

    }

    uniform_point_light_count = glGetUniformLocation(program_id, "point_light_count");

    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {

        char loc_buff[100] = { '\0' };

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].base.color", i);
        uniform_point_light[i].uniform_color = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].base.ambient_intensity", i);
        uniform_point_light[i].uniform_ambient_intensity = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].base.diffuse_intensity", i);
        uniform_point_light[i].uniform_diffuse_intensity = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].position", i);
        uniform_point_light[i].uniform_position = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].constant", i);
        uniform_point_light[i].uniform_constant = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].linear", i);
        uniform_point_light[i].uniform_linear = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].exponent", i);
        uniform_point_light[i].uniform_exponent = glGetUniformLocation(program_id, loc_buff);

        //unofrom locations for omni shadow maps
        snprintf(loc_buff, sizeof(loc_buff), "omni_shadow_maps[%zd].shadow_map", i);
        uniform_omni_shadow_map[i].uniform_shadow_map = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "omni_shadow_maps[%zd].far_plane", i);
        uniform_omni_shadow_map[i].uniform_far_plane = glGetUniformLocation(program_id, loc_buff);
    }

    for (size_t i = 0; i < MAX_MATERIALS; i++) {

        char loc_buff[100] = { '\0' };

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].ambient", i);
        uniform_material_locations[i].ambient_location = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].diffuse", i);
        uniform_material_locations[i].diffuse_location = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].specular", i);
        uniform_material_locations[i].specular_location = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].transmittance", i);
        uniform_material_locations[i].transmittance_location = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].emission", i);
        uniform_material_locations[i].emission_location = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].shininess", i);
        uniform_material_locations[i].shininess_location = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].ior", i);
        uniform_material_locations[i].ior_location = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].dissolve", i);
        uniform_material_locations[i].dissolve_location = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].illum", i);
        uniform_material_locations[i].illum_location = glGetUniformLocation(program_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "materials[%zd].textureID", i);
        uniform_material_locations[i].textureID_location = glGetUniformLocation(program_id, loc_buff);

    }

    uniform_g_clouds_location = glGetUniformLocation(program_id, "g_clouds");

    uniform_noise_texture_1_location = glGetUniformLocation(program_id, "noise_texture_1");
    uniform_noise_texture_2_location = glGetUniformLocation(program_id, "noise_texture_2");

    cloud.uniform_cloud_rad_location        = glGetUniformLocation(program_id, "cloud.rad");
    cloud.uniform_cloud_offset_location     = glGetUniformLocation(program_id, "cloud.offset");
    cloud.uniform_model_to_world            = glGetUniformLocation(program_id, "cloud.model_to_world");
    cloud.uniform_scale_location            = glGetUniformLocation(program_id, "cloud.scale");
    cloud.uniform_threshold_location        = glGetUniformLocation(program_id, "cloud.threshold");
    cloud.uniform_pillowness_location       = glGetUniformLocation(program_id, "cloud.pillowness");
    cloud.uniform_cirrus_effect_location    = glGetUniformLocation(program_id, "cloud.cirrus_effect");
    cloud.uniform_powder_effect_location    = glGetUniformLocation(program_id, "cloud.powder_effect");

    uniform_cloud_texture_location          = glGetUniformLocation(program_id, "cloud_position_depth");


    //set an array of transormation matrices for the cascade
    for (size_t i = 0; i < NUM_CASCADES; i++) {

        char loc_buff[100] = { '\0' };
        snprintf(loc_buff, sizeof(loc_buff), "directional_light_transform[%zd]", i);
        uniform_directional_light_transform_locations[i] = glGetUniformLocation(program_id, loc_buff);

    }

    uniform_random_number_location = glGetUniformLocation(program_id, "random_number");

    uniform_num_active_cascades = glGetUniformLocation(program_id, "num_active_cascades");

    uniform_pcf_radius_location = glGetUniformLocation(program_id, "pcf_radius");
    
    // Check if there are any gl Errors appears
    DebugApp_ins.areErrorPrintAll("From retrieve_uniform_locations in LightingPassShaderProgram.cpp");

}

GLuint LightingPassShaderProgram::get_g_position_location()
{
    return uniform_g_postion_location;
}

GLuint LightingPassShaderProgram::get_g_normal_location()
{
    return uniform_g_normal_position;
}

GLuint LightingPassShaderProgram::get_g_albedo_location()
{
    return uniform_g_tex_color_location;
}

GLuint LightingPassShaderProgram::get_g_directional_light_position_location(GLuint index)
{
    return uniform_g_directional_light_position_locations[index];
}

GLuint LightingPassShaderProgram::get_g_frag_depth_location()
{
    return uniform_g_frag_depth_location;
}

GLuint LightingPassShaderProgram::get_uniform_cloud_threshold_location()
{
    return cloud.uniform_threshold_location;
}

GLuint LightingPassShaderProgram::get_directional_light_transform_location(GLuint index)
{
    return uniform_directional_light_transform_locations[index];
}

GLuint LightingPassShaderProgram::get_g_clouds_location()
{
    return uniform_g_clouds_location;
}

GLuint LightingPassShaderProgram::get_random_number_location()
{
    return uniform_random_number_location;
}

GLuint LightingPassShaderProgram::get_uniform_pillowness_location()
{
    return cloud.uniform_pillowness_location;
}

GLuint LightingPassShaderProgram::get_cirrus_effect_location()
{
    return cloud.uniform_cirrus_effect_location;
}

GLuint LightingPassShaderProgram::get_cloud_powderness_effect()
{
    return cloud.uniform_powder_effect_location;
}

GLuint LightingPassShaderProgram::get_uniform_num_active_cascades_location()
{
    return uniform_num_active_cascades;
}

GLuint LightingPassShaderProgram::get_uniform_pcf_radius_location()
{
    return uniform_pcf_radius_location;
}

GLuint LightingPassShaderProgram::get_directional_shadow_map_location(GLuint index)
{
    return uniform_directional_shadow_map_locations[index];
}

GLuint LightingPassShaderProgram::get_eye_position_location()
{
    return uniform_eye_position_location;
}

GLuint LightingPassShaderProgram::get_cascade_endpoint_location(GLuint index)
{
    return uniform_cascade_endpoints_locations[index];
}

GLuint LightingPassShaderProgram::get_uniform_omni_dir_shadow_map_location(GLuint index)
{
    return uniform_omni_shadow_map[index].uniform_shadow_map;
}

GLuint LightingPassShaderProgram::get_uniform_point_light_far_plane_location(GLuint index)
{
    return uniform_omni_shadow_map[index].uniform_far_plane;
}

ObjMaterialLocations LightingPassShaderProgram::get_uniform_material_locations(GLuint index)
{
    return uniform_material_locations[index];
}



GLuint LightingPassShaderProgram::get_uniform_material_id_location()
{
    return uniform_g_material_id_location;
}

GLuint LightingPassShaderProgram::get_uniform_cloud_rad_location()
{
    return cloud.uniform_cloud_rad_location;
}

GLuint LightingPassShaderProgram::get_uniform_cloud_offset()
{
    return cloud.uniform_cloud_offset_location;
}

GLuint LightingPassShaderProgram::get_uniform_cloud_model()
{
    return cloud.uniform_model_to_world;
}

GLuint LightingPassShaderProgram::get_uniform_cloud_scale_location()
{
    return cloud.uniform_scale_location;
}

void LightingPassShaderProgram::set_point_lights(   std::vector<std::shared_ptr<PointLight>>& p_light, 
                                                    unsigned int texture_unit, unsigned int offset)
{

    glUniform1i(uniform_point_light_count, static_cast<uint32_t>(p_light.size()));

    for (size_t i = 0; i < static_cast<uint32_t>(p_light.size()); i++) {

        p_light[i]->use_light(  uniform_point_light[i].uniform_ambient_intensity, uniform_point_light[i].uniform_color,
                                uniform_point_light[i].uniform_diffuse_intensity, uniform_point_light[i].uniform_position,
                                uniform_point_light[i].uniform_constant, uniform_point_light[i].uniform_linear, uniform_point_light[i].uniform_exponent);

        p_light[i]->get_omni_shadow_map()->read(texture_unit + i);

        glUniform1i(uniform_omni_shadow_map[i + offset].uniform_shadow_map, (GLint)(texture_unit + i));
        glUniform1f(uniform_omni_shadow_map[i + offset].uniform_far_plane, p_light[i]->get_far_plane());

    }

    DebugApp_ins.areErrorPrintAll("From set_point_light in LightingPassShaderProgram.cpp");

}

void LightingPassShaderProgram::set_noise_textures(GLuint start)
{

    glUniform1i(uniform_noise_texture_1_location, start);
    glUniform1i(uniform_noise_texture_2_location, start + 1);

    DebugApp_ins.areErrorPrintAll("From set_noise_textures in LightingPassShaderProgram.cpp");
}

void LightingPassShaderProgram::set_cloud_texture(GLuint index)
{
    glUniform1i(uniform_cloud_texture_location, index);
}

GLuint LightingPassShaderProgram::get_directional_light_ambient_intensity_location()
{
    return d_light_uniform_locations.uniform_ambient_intensity_location;
}

GLuint LightingPassShaderProgram::get_directional_light_color_location()
{
    return d_light_uniform_locations.uniform_color_location;
}

GLuint LightingPassShaderProgram::get_directional_light_diffuse_intensity_location()
{
    return d_light_uniform_locations.uniform_diffuse_intensity_location;
}

GLuint LightingPassShaderProgram::get_directional_light_direction_location()
{
    return d_light_uniform_locations.uniform_direction_location;
}

GLuint LightingPassShaderProgram::get_directional_light_shadow_intensity_location()
{
    return d_light_uniform_locations.uniform_shadow_intensity_location;
}


LightingPassShaderProgram::~LightingPassShaderProgram()
{
}
