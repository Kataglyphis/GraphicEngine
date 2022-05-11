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

    void execute(   glm::mat4 projection_matrix, 
                    std::shared_ptr<Camera> main_camera,
                    GLfloat window_width, GLfloat window_height, 
                    GLuint gbuffer_id, GLfloat delta_time, 
                    std::shared_ptr<Scene>);

    void create_shader_program();

    void set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model);

    ~GeometryPass();

private:

    void retrieve_geometry_pass_locations(  glm::mat4 projection_matrix, 
                                            glm::mat4 view_matrix, 
                                            std::vector<ObjMaterial>& materials,
                                            std::shared_ptr<Scene> scene);

    std::shared_ptr<GeometryPassShaderProgram> shader_program;

    SkyBox          skybox;
    // TO check if there are any gl error
    DebugApp        DebugApp_ins;
    UniformHelper   uniform_helper;

};

