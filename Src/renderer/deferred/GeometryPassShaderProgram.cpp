#include "GeometryPassShaderProgram.h"

GeometryPassShaderProgram::GeometryPassShaderProgram()
{
}

void GeometryPassShaderProgram::retrieve_uniform_locations() {

    // check before Error executing gl function if they are gl Errors in the Queue
    DebugApp_ins.arePreError("From retrieve_uniform_locations in GeometryPassShaderProgram.");

    uniform_model_location          = glGetUniformLocation(program_id, "model");
    uniform_normal_model_location   = glGetUniformLocation(program_id, "normal_model");
    uniform_projection_location     = glGetUniformLocation(program_id, "projection");
    uniform_view_location           = glGetUniformLocation(program_id, "view");

    for (size_t i = 0; i < MAX_TEXTURE_COUNT; i++) {

        char loc_buff[100] = { '\0' };
        snprintf(loc_buff, sizeof(loc_buff), "model_textures[%zd]", i);
        uniform_texture_locations[i] = glGetUniformLocation(program_id, loc_buff);
        
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

    // Check if everythin went well.
    DebugApp_ins.areErrorPrintAll("From retrieve_uniform_locations in GeometryPassShaderProgram.");
    
}

GLuint GeometryPassShaderProgram::get_projection_location() {
    return uniform_projection_location;
}

GLuint GeometryPassShaderProgram::get_view_location() {
    return uniform_view_location;
}


GLuint GeometryPassShaderProgram::get_model_location() {
    return uniform_model_location;
}

GLuint GeometryPassShaderProgram::get_normal_modal_location()
{
    return uniform_normal_model_location;
}

ObjMaterialLocations GeometryPassShaderProgram::get_uniform_material_locations(int index)
{
    return uniform_material_locations[index];
}

GLuint GeometryPassShaderProgram::get_uniform_texture_locations(int index)
{
    return uniform_texture_locations[index];
}

GeometryPassShaderProgram::~GeometryPassShaderProgram()
{
}