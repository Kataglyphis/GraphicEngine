#pragma once
#include "RenderPassSceneDependend.h"
#include "DirectionalLight.h"
#include "GeometryPassShaderProgram.h"
#include "SkyBox.h"
#include "ViewFrustumCulling.h"
#include "Camera.h"
#include "Clouds.h"
#include "Scene.h"
#include "SkyBox.h"
#include <array>

#include "UniformHelper.h"

#include "DebugApp.h"

class GeometryPass :
    public RenderPassSceneDependend
{
public:

    GeometryPass();
    GeometryPass(std::shared_ptr<GeometryPassShaderProgram> shader_program);

    void execute(glm::mat4 projection_matrix, glm::mat4 view_matrix,  GLfloat window_width, GLfloat window_height, 
                            GLuint gbuffer_id, bool first_person_mode, GLfloat delta_time, Scene* scene);

    void set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model);

    ~GeometryPass();

private:

    void retrieve_geometry_pass_locations(  glm::mat4 projection_matrix, glm::mat4 view_matrix, 
                                            std::vector<ObjMaterial>& materials,
                                            Scene* scene);

    std::shared_ptr<GeometryPassShaderProgram> shader_program;

    SkyBox skybox;

    // TO check if there are any gl error
    DebugApp DebugApp_ins;
    UniformHelper uniform_helper;

};

