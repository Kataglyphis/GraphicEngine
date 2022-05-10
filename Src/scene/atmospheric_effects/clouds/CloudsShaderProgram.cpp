#include "CloudsShaderProgram.h"

CloudsShaderProgram::CloudsShaderProgram()
{
}

GLuint CloudsShaderProgram::get_projection_location()
{
    return uniform_projection_location;
}

GLuint CloudsShaderProgram::get_view_location()
{
    return uniform_view_location;
}

GLuint CloudsShaderProgram::get_model_location()
{
    return uniform_model_location;
}

GLuint CloudsShaderProgram::get_clouds_material_id_location()
{
    return uniform_clouds_material_id_location;
}

CloudsShaderProgram::~CloudsShaderProgram()
{
}

void CloudsShaderProgram::retrieve_uniform_locations()
{
    uniform_model_location              = glGetUniformLocation(program_id, "model");
    uniform_projection_location         = glGetUniformLocation(program_id, "projection");
    uniform_view_location               = glGetUniformLocation(program_id, "view");
    uniform_clouds_material_id_location = glGetUniformLocation(program_id, "cloudsMaterialID");
}
