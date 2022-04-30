#include "GeometryPassShaderProgram.h"

GeometryPassShaderProgram::GeometryPassShaderProgram()
{
}
void GeometryPassShaderProgram::retrieve_uniform_locations() {

    // check before Error executing gl function if they are gl Errors in the Queue
    glErrorChecker_ins.arePreError("From retrieve_uniform_locations in GeometryPassShaderProgram.");

    uniform_model_location          = glGetUniformLocation(program_id, "model");
    uniform_normal_model_location   = glGetUniformLocation(program_id, "normal_model");
    uniform_projection_location     = glGetUniformLocation(program_id, "projection");
    uniform_view_location           = glGetUniformLocation(program_id, "view");

    for (size_t i = 0; i < MAX_TEXTURE_COUNT; i++) {

        char loc_buff[100] = { '\0' };
        snprintf(loc_buff, sizeof(loc_buff), "model_textures[%zd]", i);
        uniform_texture_locations[i] = glGetUniformLocation(program_id, loc_buff);
        glUniform1i(uniform_texture_locations[i],i);
    }

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

GeometryPassShaderProgram::~GeometryPassShaderProgram()
{
}