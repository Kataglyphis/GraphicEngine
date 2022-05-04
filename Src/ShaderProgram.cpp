#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
    program_id = 0;
}

void ShaderProgram::create_from_files(const char* vertex_location, const char* fragment_location)
{
    
    std::stringstream vertex_shader;
    std::stringstream fragment_shader;
    vertex_shader << shader_base_dir << vertex_location;
    fragment_shader << shader_base_dir << fragment_location;

    std::string vertex_string = read_file(vertex_shader.str().c_str());
    std::string fragment_string = read_file(fragment_shader.str().c_str());

    //we need c-like strings ....
    const char* vertex_code = vertex_string.c_str();
    const char* fragment_code = fragment_string.c_str();

    this->vertex_location = (vertex_location);
    this->fragment_location = (fragment_location);

    compile_shader_program(vertex_code, fragment_code);
}

void ShaderProgram::create_from_files(const char* vertex_location, const char* geometry_location, const char* fragment_location)
{

    std::stringstream vertex_shader;
    std::stringstream geometry_shader;
    std::stringstream fragment_shader;
    vertex_shader << shader_base_dir << vertex_location;
    geometry_shader << shader_base_dir << geometry_location;
    fragment_shader << shader_base_dir << fragment_location;

    std::string vertex_string   = read_file(vertex_shader.str().c_str());
    std::string geometry_string = read_file(geometry_shader.str().c_str());
    std::string fragment_string = read_file(fragment_shader.str().c_str());


    const char* vertex_code = vertex_string.c_str();
    const char* geometry_code = geometry_string.c_str();
    const char* fragment_code = fragment_string.c_str();

    this->vertex_location = vertex_location;
    this->fragment_location = fragment_location;
    this->geometry_location = geometry_location;

    compile_shader_program(vertex_code, geometry_code, fragment_code);
}

void ShaderProgram::create_computer_shader_program_from_file(const char* compute_location)
{
    std::string file = read_file(compute_location);
    const char* compute_code = file.c_str();

    this->compute_location = compute_location;

    compile_compute_shader_program(compute_code);
}

GLuint ShaderProgram::get_id()
{
    return program_id;
}

void ShaderProgram::validate_program()
{
    // check if there are any gl error before executing gl function
    DebugApp_ins.arePreError("From validate_program in ShaderProgram.cpp");
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glValidateProgram(program_id);

    glGetProgramiv(program_id, GL_VALIDATE_STATUS, &result);

    DebugApp_ins.areErrorPrintAll("From validate_program in ShaderProgram.cpp");

    if (!result) {
        glGetProgramInfoLog(program_id, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

}

void ShaderProgram::reload()
{
    create_from_files(this->vertex_location, this->fragment_location);
}

void ShaderProgram::use_shader_program()
{
    glUseProgram(program_id);
}


std::string ShaderProgram::read_file(const char* file_location)
{
    std::string content;
    std::ifstream file_stream(file_location, std::ios::in);

    if (!file_stream.is_open()) {
        printf("Failed to read %s. File does not exist.", file_location);
        return "";
    }

    std::string line = "";
    while (!file_stream.eof()) {
        std::getline(file_stream, line);
        content.append(line + "\n");
    }

    file_stream.close();
    return content;
}

void ShaderProgram::add_shader(GLuint program, const char* shader_code, GLenum shader_type)
{
    GLuint shader = glCreateShader(shader_type);

    // the opengl function wants c -style char array of code and the length in an array ... so we do it
    const GLchar* code[1];
    code[0] = shader_code;

    GLint code_length[1];
    code_length[0] = (GLint)strlen(shader_code);

    glShaderSource(shader, 1, code, code_length);
    glCompileShader(shader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    //retrieve status of the shader and print if any error occured
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (!result) {
        glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader:  '%s'\n", shader_type, eLog);
        printf("%s",shader_code);
        return;
    }

    // we are happy, everything went well so attach shader to program
    glAttachShader(program, shader);
}

void ShaderProgram::compile_shader_program(const char* vertex_code, const char* fragment_code)
{
    // retrieve the id; we need to reference it later on
    program_id = glCreateProgram();

    if (!program_id) {
        printf("Error creating shader program !\n");
        return;
    }
    // we will always need one vertex ShaderProgram
    add_shader(program_id, vertex_code, GL_VERTEX_SHADER);
    // and one fragment ShaderProgram
    add_shader(program_id, fragment_code, GL_FRAGMENT_SHADER);

    //we attached all shaders
    //so compile program
    compile_program();
}

void ShaderProgram::compile_shader_program(const char* vertex_code, const char* geometry_code, const char* fragment_code)
{
    program_id = glCreateProgram();

    if (!program_id) {
        printf("Error creating shader program!\n");
        return;
    }

    add_shader(program_id, vertex_code, GL_VERTEX_SHADER);
    add_shader(program_id, geometry_code, GL_GEOMETRY_SHADER);
    add_shader(program_id, fragment_code, GL_FRAGMENT_SHADER);

    compile_program();
}

void ShaderProgram::compile_compute_shader_program(const char* compute_code)
{
    program_id = glCreateProgram();

    if (!program_id) {
        printf("Error creating shader program!\n");
        return;
    }

    add_shader(program_id, compute_code, GL_COMPUTE_SHADER);

    compile_program();
}

void ShaderProgram::compile_program()
{
    //as simple as that; opengl will link it for us :)
    glLinkProgram(program_id);
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetProgramiv(program_id, GL_LINK_STATUS, &result);

    if (!result) {
        glGetProgramInfoLog(program_id, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    validate_program();
    //after linking we are able to retrieve all the positions of the uniforms in our shader program :) 
    retrieve_uniform_locations();
}

ShaderProgram::~ShaderProgram()
{
    clear_shader_program();
}

void ShaderProgram::clear_shader_program()
{
    //don't trash the id's!!
    //delete it from memory!!
    if (program_id != 0) {
        glDeleteProgram(program_id);
        program_id = 0;
    }
}
