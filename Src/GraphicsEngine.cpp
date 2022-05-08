//necessary for the stb header file ....
#define STB_IMAGE_IMPLEMENTATION

// include ability to execute threads
#include <thread>
#include <mutex>
#include <array>
#include <memory>
#include <sstream>

//all vector math helper includes :)
#include <limits>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "DebugApp.h"
#include "File.h"
#include "GUI.h"
#include "LoadingScreen.h"

// all shader programs
#include "ShaderProgram.h"
#include "GeometryPassShaderProgram.h"
#include "LightingPassShaderProgram.h"
#include "ShadowMapShaderProgram.h"
#include "OmniDirShadowShaderProgram.h"
#include "DirectionalLightUniformLocations.h"

//all render passes
#include "DirectionalShadowMapPass.h"
#include "OmniShadowMapPass.h"
#include "LightingPass.h"
#include "GeometryPass.h"
#include "GBuffer.h"
#include "Quad.h"

//all for environment effects
#include "Noise.h"
#include "Clouds.h"

//all scene/game logic/ game object related stuff
#include "Scene.h"
#include "GameObject.h"
#include "Texture.h"
#include "ObjMaterial.h"
#include "Mesh.h"
#include "Model.h"
#include "ViewFrustumCulling.h"
#include "Window.h"
#include "Camera.h"

#include "GlobalValues.h"
#include "host_device_shared.h"

//our global variables
GLfloat delta_time = 0.0f;
GLfloat last_time = 0.0f;

std::shared_ptr<GBuffer> gbuffer;
std::shared_ptr<Noise> noise;
std::shared_ptr<Clouds> clouds;

// we will need a bunch of shader programs
std::shared_ptr<GeometryPassShaderProgram> g_buffer_geometry_pass_shader_program;
std::shared_ptr<LightingPassShaderProgram> g_buffer_lighting_pass_shader_program;
std::shared_ptr<ShadowMapShaderProgram> shadow_map_shader_program;
std::shared_ptr<OmniDirShadowShaderProgram> omni_dir_shadow_shader_program;

OmniShadowMapPass omni_shadow_map_pass;
DirectionalShadowMapPass directional_shadow_map_pass;
GeometryPass geometry_pass;
LightingPass lighting_pass;

unsigned int material_counter = 0;

bool loading_screen_finished = false;

// https://cpp.hotexamples.com/examples/-/-/glNamedStringARB/cpp-glnamedstringarb-function-examples.html
void set_shader_includes() {
    
    // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_shading_language_include.txt
    DebugApp DebugApp_ins;

    const int num_include_files = 10;
    std::array<const char*, num_include_files> includeNames     = { "host_device_shared.h",
                                                                    "Globals.glsl",
                                                                    "Matlib.glsl",
                                                                    "microfacet.glsl",
                                                                    "ShadingLibrary.glsl",
                                                                    "disney.glsl",
                                                                    "frostbite.glsl",
                                                                    "pbrBook.glsl",
                                                                    "phong.glsl",
                                                                    "unreal4.glsl"
                                                                    };

    std::array<const char*, num_include_files> file_locations_relative   = {    "/Src/host_device_shared.h",
                                                                                "/Resources/Shaders/common/Globals.glsl",
                                                                                "/Resources/Shaders/common/Matlib.glsl",
                                                                                "/Resources/Shaders/common/microfacet.glsl",
                                                                                "/Resources/Shaders/common/ShadingLibrary.glsl" ,
                                                                                "/Resources/Shaders/brdf/disney.glsl",
                                                                                "/Resources/Shaders/brdf/frostbite.glsl",
                                                                                "/Resources/Shaders/brdf/pbrBook.glsl",
                                                                                "/Resources/Shaders/brdf/phong.glsl",
                                                                                "/Resources/Shaders/brdf/unreal4.glsl"
                                                                                };
    std::vector<std::string> file_locations_abs;
    for (int i = 0; i < includeNames.size(); i++) {

        std::stringstream aux;
        aux << CMAKELISTS_DIR;
        aux << file_locations_relative[i]; 
        file_locations_abs.push_back(aux.str());
    }




    for (int i = 0; i < num_include_files; i++) {

        File file(file_locations_abs[i]);
        std::string file_content = file.read();
        char tmpstr[200];
        sprintf(tmpstr, "/%s", includeNames[i]);
        glNamedStringARB(GL_SHADER_INCLUDE_ARB, strlen(tmpstr), tmpstr, strlen(file_content.c_str()), file_content.c_str());
        DebugApp_ins.areErrorPrintAll("From glNamedStringARB.");

    }

}



void create_geometry_pass_shader_program() {

    g_buffer_geometry_pass_shader_program = std::make_shared<GeometryPassShaderProgram>(GeometryPassShaderProgram{});
    g_buffer_geometry_pass_shader_program->create_from_files(   "rasterizer/g_buffer_geometry_pass.vert", 
                                                                "rasterizer/g_buffer_geometry_pass.frag");

}

void create_lighting_pass_shader_program() {

    g_buffer_lighting_pass_shader_program = std::make_shared<LightingPassShaderProgram>(LightingPassShaderProgram{});
    g_buffer_lighting_pass_shader_program->create_from_files(   "rasterizer/g_buffer_lighting_pass.vert",
                                                                "rasterizer/g_buffer_lighting_pass.frag");

}

void create_shadow_map_shader_program() {

    shadow_map_shader_program = std::make_shared<ShadowMapShaderProgram>(ShadowMapShaderProgram{});
    shadow_map_shader_program->create_from_files(   "rasterizer/shadows/directional_shadow_map.vert",
                                                    "rasterizer/shadows/directional_shadow_map.geom",
                                                    "rasterizer/shadows/directional_shadow_map.frag");

}

void create_omni_shadow_map_shader_program() {

    

    omni_dir_shadow_shader_program = std::make_shared<OmniDirShadowShaderProgram>(OmniDirShadowShaderProgram{});
    omni_dir_shadow_shader_program->create_from_files(  "rasterizer/shadows/omni_shadow_map.vert",
                                                        "rasterizer/shadows/omni_shadow_map.geom", 
                                                        "rasterizer/shadows/omni_shadow_map.frag");

}

// here we will initialize all our programs
void create_shader_programs() {

    // set include file for shaders
    set_shader_includes();
    create_geometry_pass_shader_program();
    create_lighting_pass_shader_program();
    create_shadow_map_shader_program();
    create_omni_shadow_map_shader_program();

}

void reload_shader_programs()
{
    set_shader_includes();
    shadow_map_shader_program->reload();
    g_buffer_geometry_pass_shader_program->reload();
    g_buffer_lighting_pass_shader_program->reload();
    omni_dir_shadow_shader_program->reload();
    clouds->get_shader_program()->reload();

}

void create_noise_textures() {

    noise->create_worley_noise();
    noise->create_grad_noise();

}

void reload_noise_programs() {

    noise->update();
    create_noise_textures();
}


int main()
{
    GLint window_width = 1200;
    GLint window_height = 800;
    
    // make sure ti initialize window first
    // this will create opengl context!
    Window main_window;
    main_window = Window(window_width, window_height);
    main_window.initialize();

    GUI gui;
    LoadingScreen loading_screen; 

    noise = std::make_shared<Noise>();
    noise->init();

    clouds = std::make_shared<Clouds>();
    clouds->init(window_width, window_height);

    std::shared_ptr<Camera> main_camera = std::make_shared<Camera>( glm::vec3(0.0f,50.0f,0.0f), 
                                                                    glm::vec3(0.0f, 1.0f, 0.0f), 
                                                                    -60.0f,
                                                                    0.0f, 35.0f, 0.25f,
                                                                    0.1f, 500.f, 45.f);

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    scene->init(main_camera, &main_window, clouds);

    gbuffer = std::make_shared<GBuffer>(window_width, window_height);
    gbuffer->create();
    

    //create shader programs and use the standard shader
    create_shader_programs();

    //after creati gprograms one can init render passes
    omni_shadow_map_pass        = OmniShadowMapPass(omni_dir_shadow_shader_program);
    directional_shadow_map_pass = DirectionalShadowMapPass(shadow_map_shader_program);
    geometry_pass               = GeometryPass(g_buffer_geometry_pass_shader_program);
    //lighting_pass = LightingPass();
    lighting_pass.init(g_buffer_lighting_pass_shader_program);
    //precompute our noise textures ones
    create_noise_textures();

    gui.init(main_window);

    //enable depth testing
    glEnable(GL_DEPTH_TEST);

    //init texture for loading screen
    loading_screen.init();

    std::thread t1 = scene->spwan();
    t1.detach();

    while (!main_window.get_should_close()) {

        glViewport(0, 0, window_width, window_height);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //we need the projection matrix, just use glm::perspective function
        glm::mat4 projection_matrix = glm::perspectiveFov(  glm::radians(main_camera->get_fov()),
                                                            (GLfloat)window_width, (GLfloat)window_height,
                                                            main_camera->get_near_plane(), main_camera->get_far_plane());

        //we should make the application independet of processor speed :)
        // take time into account is crucial
        // concept of delta time: https://bell0bytes.eu/keeping-track-of-time/
        GLfloat now = (float)glfwGetTime();
        delta_time = now - last_time;
        last_time = now;

        //poll all events incoming from user
        glfwPollEvents();

        // handle events for the camera
        main_camera->key_control(main_window.get_keys(), delta_time);
        main_camera->mouse_control(main_window.get_x_change(), main_window.get_y_change());

        if (scene->is_loaded()) {

            if (!loading_screen_finished) {

                loading_screen_finished = true;

            }

            if(!scene->get_context_setup()) scene->setup_game_object_context();

            directional_shadow_map_pass.execute(projection_matrix, 
                                                main_camera,
                                                window_width, window_height,
                                                scene);

            // omni shadow map passes for our point lights
            std::vector<std::shared_ptr<PointLight>> p_lights = scene->get_point_lights();
            for (size_t p_light_count = 0; p_light_count < scene->get_point_light_count(); p_light_count++) {
                omni_shadow_map_pass.execute(p_lights[p_light_count], scene);
            }

            //we will now start the geometry pass
            geometry_pass.execute(  projection_matrix, main_camera->calculate_viewmatrix(), 
                                    window_width, window_height, gbuffer->get_id(),
                                    delta_time, scene);

            // render the AABB for the clouds
            clouds->render( projection_matrix, main_camera->calculate_viewmatrix(), 
                            window_width, window_height);

            // after geometry pass we can now do the lighting
            lighting_pass.execute(  projection_matrix, 
                                    main_camera,
                                    scene,
                                    gbuffer,
                                    noise, 
                                    clouds, 
                                    delta_time);

        }
        else {

            // play the audio
            //SoundEngine->play2D("Audio/Red_Dead_Redemption_2 _Loading_Screen.mp3", true); //
            loading_screen.render();

        }

        bool shader_hot_reload_triggered = false;
        bool noise_hot_reload_triggered = false;
        gui.render(!scene->is_loaded(), scene->get_progress(), shader_hot_reload_triggered, noise_hot_reload_triggered);

        if(shader_hot_reload_triggered) reload_shader_programs();
        if(noise_hot_reload_triggered) reload_noise_programs();

        gui.update_user_input(scene, clouds);

        main_window.update_viewport();
        GLfloat new_window_width = main_window.get_buffer_width();
        GLfloat new_window_height = main_window.get_buffer_height();

        if ((new_window_width == window_width && window_height == new_window_height) == false) {

            window_height = new_window_height;
            window_width = new_window_width;
            gbuffer->update_window_params(window_width, window_height);
            gbuffer->create();
            clouds->update_window_params(window_width, window_height);

        }

        main_window.swap_buffers();
    }

}



