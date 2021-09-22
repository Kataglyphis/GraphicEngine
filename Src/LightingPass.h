#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LightingPassShaderProgram.h"
#include "Quad.h"
#include "GBuffer.h"
#include "Material.h"
#include "Noise.h"
#include "Clouds.h"
#include "Quad.h"

#include <ctime>
#include <chrono>
#include <random>
#include <cassert>
#include <time.h>  

class LightingPass 
{
public:

    LightingPass();

    void init(std::shared_ptr<LightingPassShaderProgram> shader_program);

    void execute(glm::mat4 projection_matrix, glm::mat4 view_matrix, std::shared_ptr<GBuffer> gbuffer, std::shared_ptr<DirectionalLight> main_light, 
                            std::vector<std::shared_ptr<PointLight>>& point_lights, GLuint point_light_count, glm::vec3 camera_position, GLuint material_counter,
                            std::vector<std::shared_ptr<Material>>& materials, std::shared_ptr<Noise> noise, std::shared_ptr<Clouds> cloud, float delta_time);

    ~LightingPass();

private:

    glm::vec3 current_offset;

    GLfloat* random_number_data;

    GLuint random_number;

    void generate_random_numbers();

    void retrieve_lighting_pass_locations(glm::mat4 projection_matrix, glm::mat4 view_matrix, 
                                                                        std::shared_ptr<GBuffer> gbuffer,
                                                                        std::shared_ptr<DirectionalLight> main_light,
                                                                        std::vector<std::shared_ptr<PointLight>>& point_lights, GLuint point_light_count, glm::vec3 camera_position, GLuint material_counter,
                                                                        std::vector<std::shared_ptr<Material>>& materials, std::shared_ptr<Clouds> cloud, float delta_time);

    void bind_buffers_for_lighting(std::shared_ptr<GBuffer> gbuffer, std::shared_ptr<DirectionalLight> main_light, std::shared_ptr<Noise> noise, GLuint point_light_count, std::shared_ptr<Clouds> cloud);

    void bind_random_numbers(GLuint texture_unit);

    std::shared_ptr<LightingPassShaderProgram> shader_program;

    Quad quad;

};

