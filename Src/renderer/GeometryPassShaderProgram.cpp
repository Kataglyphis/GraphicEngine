#include "GeometryPassShaderProgram.h"

GeometryPassShaderProgram::GeometryPassShaderProgram()
{
}
void GeometryPassShaderProgram::retrieve_uniform_locations() {

    // check before Error executing gl function if they are gl Errors in the Queue
    glErrorChecker_ins.arePreError("From retrieve_uniform_locations in GeometryPassShaderProgram.");

    uniform_model_location = glGetUniformLocation(program_id, "model");
    uniform_normal_model_location = glGetUniformLocation(program_id, "normal_model");
    uniform_projection_location = glGetUniformLocation(program_id, "projection");
    uniform_view_location = glGetUniformLocation(program_id, "view");

    uniform_material_id_location = glGetUniformLocation(program_id, "material_id");

    // Check if everythin went well.
    glErrorChecker_ins.areErrorPrintAll("From retrieve_uniform_locations in GeometryPassShaderProgram.");
    
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

GLuint GeometryPassShaderProgram::get_material_id_location()
{
    return uniform_material_id_location;
}

GLuint GeometryPassShaderProgram::get_terrain_id()
{
    return uniform_isTerrainValue_id;
}

GLuint GeometryPassShaderProgram::get_biom_height_id()
{
    return uniform_biomHeight_id;
}

GLuint GeometryPassShaderProgram::get_max_height_id()
{
    return uniform_max_height_id;
}

GeometryPassShaderProgram::~GeometryPassShaderProgram()
{
}