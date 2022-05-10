#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LightingPassShaderProgram.h"
#include "Quad.h"
#include "GBuffer.h"
#include "ObjMaterial.h"
#include "Noise.h"
#include "Clouds.h"
#include "Quad.h"
#include "Camera.h"
#include "Scene.h"

#include <ctime>
#include <chrono>
#include <cassert>
#include <time.h>  
#include <memory>

#include "RandomNumbers.h"
#include "UniformHelper.h"

class LightingPass : public RenderPass
{
public:

    LightingPass();

    void init();

    void execute(   glm::mat4 projection_matrix, 
                    std::shared_ptr<Camera>,
                    std::shared_ptr<Scene> scene,
                    std::shared_ptr<GBuffer> gbuffer, 
                    float delta_time);

    void create_shader_program();

    ~LightingPass();

private:

    glm::vec3 current_offset;

    RandomNumbers random_numbers;
    std::shared_ptr<GLfloat[]> random_number_data;

    GLuint random_number;

    void retrieve_lighting_pass_locations(  glm::mat4 projection_matrix,
                                            std::shared_ptr<Camera> main_camera,
                                            std::shared_ptr<Scene> scene,
                                            std::shared_ptr<GBuffer> gbuffer,
                                            float delta_time);

    void bind_buffers_for_lighting( std::shared_ptr<GBuffer> gbuffer, 
                                    std::shared_ptr<Scene> scene, 
                                    std::shared_ptr<Clouds> cloud);

    void bind_random_numbers(GLuint texture_unit);

    std::shared_ptr<LightingPassShaderProgram> shader_program;

    UniformHelper uniform_helper;

    Quad quad;

};

