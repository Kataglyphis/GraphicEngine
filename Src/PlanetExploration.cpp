#pragma once
//necessary for the stb header file ....
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

// include ability to execute threads 
#include <thread>
#include <mutex>

#include <memory>

//all vector math helper includes :)
#include <limits>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//all gui stuff
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// all shader programs
#include "ShaderProgram.h"
#include "GeometryPassShaderProgram.h"
#include "LightingPassShaderProgram.h"
#include "ShadowMapShaderProgram.h"
#include "OmniDirShadowShaderProgram.h"
#include "DirectionalLightUniformLocations.h"
#include "LoadingScreenShaderProgram.h"

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
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "Terrain_generator.h"
#include "ViewFrustumCulling.h"
#include "MyWindow.h"
#include "Camera.h"

#include "GlobalValues.h"

//our global variables
GLfloat delta_time = 0.0f;
GLfloat last_time = 0.0f;

unsigned int point_light_count = 0;

//define near and far plane
GLfloat near_plane = 0.1f;
GLfloat far_plane = 500.f;
GLfloat far_plane_shadow = 900.f;

//shadow map var
GLuint shadow_map_resolution = 4096;

//in degrees; , shadow and view
GLfloat fov= 60.0f;

//global objects
std::shared_ptr<MyWindow> main_window;
std::shared_ptr<Camera> main_camera;
DirectionalLight main_light;
PointLight point_lights[MAX_POINT_LIGHTS];

Texture ornament1;
Material materials[MAX_MATERIALS];
GBuffer gbuffer;

Noise noise;
std::shared_ptr<Clouds> clouds;
Scene scene;

// we will need a bunch of shader programs
std::shared_ptr<GeometryPassShaderProgram> g_buffer_geometry_pass_shader_program;
std::shared_ptr<LightingPassShaderProgram> g_buffer_lighting_pass_shader_program;
std::shared_ptr<ShadowMapShaderProgram> shadow_map_shader_program;
std::shared_ptr<OmniDirShadowShaderProgram> omni_dir_shadow_shader_program;
std::shared_ptr<LoadingScreenShaderProgram> loading_screen_shader_program;

OmniShadowMapPass omni_shadow_map_pass;
DirectionalShadowMapPass directional_shadow_map_pass;
GeometryPass geometry_pass;
LightingPass lighting_pass;

std::shared_ptr<Terrain_Generator> tGenerator;

//everything necessary for the loading screen
Quad loading_screen;
Texture loading_screen_tex;
Texture logo;

// all variables for gui 
glm::vec3 directional_light_starting_position = glm::vec3(0.0f, -1.0f, 0.1f);
glm::vec3 directional_light_starting_color = glm::vec3(1.0f);

static float direcional_light_ambient_intensity = 4.0f;
static float direcional_light_diffuse_intensity = 0.5f;

static float directional_light_color[3] = {directional_light_starting_color.x,
                                                                        directional_light_starting_color.y,
                                                                        directional_light_starting_color.z };

static float directional_light_direction[3] = { directional_light_starting_position.x, 
                                                                                directional_light_starting_position.y, 
                                                                                directional_light_starting_position.z };

unsigned int material_counter = 0;
static bool first_person_mode = false;
bool ssao_enabled = false;
bool ssr_enabled = false;

int cloud_speed = 6;
float cloud_scale = 0.63f;
float cloud_density = 0.667f;
float cloud_pillowness = 0.966f;
float cloud_cirrus_effect = 0.0f;
float cloud_mesh_scale[3] = { 1000.f, 20.f, 1000.f }; 
bool cloud_powder_effect = false;
float cloud_movement_direction [3] = { 1.f, 1.f, 1.f };

float sound_volume = 0.0f;

int choosen_space_ship = 1;

bool loading_screen_finished = false;

//terrain variables
int terrain_height = 32;

// shadow vars
int shadow_map_res_index = 3;
bool shadow_resolution_changed = false;
int num_shadow_cascades = NUM_MIN_CASCADES;
int pcf_radius = 2;
float cascaded_shadow_intensity = 0.65f;
const char* available_shadow_map_resolutions[] = { "512","1024","2048", "4096"};

void create_geometry_pass_shader_program() {

    g_buffer_geometry_pass_shader_program = std::make_shared<GeometryPassShaderProgram>(GeometryPassShaderProgram{});
    g_buffer_geometry_pass_shader_program->create_from_files("Shaders/g_buffer_geometry_pass.vert", "Shaders/g_buffer_geometry_pass.frag");

}

void create_lighting_pass_shader_program() {
    
    g_buffer_lighting_pass_shader_program = std::make_shared<LightingPassShaderProgram>(LightingPassShaderProgram{});
    g_buffer_lighting_pass_shader_program->create_from_files("Shaders/g_buffer_lighting_pass.vert", "Shaders/g_buffer_lighting_pass.frag");

}

void create_shadow_map_shader_program() {

    shadow_map_shader_program = std::make_shared<ShadowMapShaderProgram>(ShadowMapShaderProgram{});
    shadow_map_shader_program->create_from_files("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
}

void create_omni_shadow_map_shader_program() {

    omni_dir_shadow_shader_program = std::make_shared<OmniDirShadowShaderProgram>(OmniDirShadowShaderProgram{});
    omni_dir_shadow_shader_program->create_from_files("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");

}

void create_loading_screen_shader_program() {

    loading_screen_shader_program = std::make_shared<LoadingScreenShaderProgram>( LoadingScreenShaderProgram{});
    loading_screen_shader_program->create_from_files("Shaders/loading_screen.vert", "Shaders/loading_screen.frag");

}

// here we will initialize all our programs
void create_shader_programs() {

    create_geometry_pass_shader_program();
    create_lighting_pass_shader_program();
    create_shadow_map_shader_program();
    create_omni_shadow_map_shader_program();
    create_loading_screen_shader_program();

}

void reload_shader_programs()
{

    shadow_map_shader_program->reload();
    g_buffer_geometry_pass_shader_program->reload();
    g_buffer_lighting_pass_shader_program->reload();
    omni_dir_shadow_shader_program->reload();
    clouds->get_shader_program()->reload();

}

void create_noise_textures() {

    noise.create_worley_noise();
    noise.create_grad_noise();

}

void reload_noise_programs() {

    noise.update();
    create_noise_textures();
}


int main()
{

    GLint window_width = 1900;
    GLint window_height = 1020;

    main_window = std::make_shared<MyWindow>(MyWindow(window_width, window_height));
    main_window->initialize();

    noise.init();

    tGenerator = std::make_shared<Terrain_Generator>(Terrain_Generator());

    clouds = std::make_shared<Clouds>(Clouds());
    clouds->init(window_width, window_height, cloud_speed);

    //play with the speeds ! :) Kansei changed the speed to 75 ;) 
    main_camera = std::make_shared<Camera>(Camera(glm::vec3(0.0f,50.0f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f,
                                            0.0f, 75.0f, 0.25f, 
                                            near_plane, far_plane, fov));

    //scene = Scene();
    scene.init(main_camera, main_window, tGenerator, clouds);
   
    gbuffer = GBuffer(window_width, window_height);
    gbuffer.create();


    
    //initialize main dir light
    main_light = DirectionalLight ( shadow_map_resolution,
                                                        shadow_map_resolution,
                                                        directional_light_starting_color.x,
                                                        directional_light_starting_color.y,
                                                        directional_light_starting_color.z,
                                                        direcional_light_ambient_intensity,
                                                        direcional_light_diffuse_intensity,
                                                        directional_light_starting_position.x,
                                                        directional_light_starting_position.y,
                                                        directional_light_starting_position.z,
                                                        main_camera->get_near_plane(), main_camera->get_far_plane(),
                                                        far_plane_shadow, num_shadow_cascades );

    point_lights[0] = PointLight(1024, 1024,
                                    0.01f, 100.f,
                                    0.0f, 1.0f, 0.0f,
                                    1.f, 1.0f,
                                    0.0f, 0.0f, 0.0f,
                                    0.1f, 0.1f, 0.1f);

    point_light_count++;

    point_lights[1] = PointLight(1024, 1024,
        0.01f, 100.f,
        1.0f, 0.0f, 0.0f,
        1.f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.1f, 0.1f, 0.1f);

    point_light_count++;

    point_lights[2] = PointLight(1024, 1024,
        0.01f, 100.f,
        0.0f, 0.0f, 1.0f,
        1.f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.1f, 0.1f, 0.1f);

    point_light_count++;

    GLfloat silizium_carbide_IOR = 2.65f;
    GLfloat water_IOR = 2.65f;

    GLfloat absorption_coeff_cond = 4.f;
    GLfloat absorption_coeff_diel = 0.0f;

    materials[0] = Material(1.0f, 0.1f, silizium_carbide_IOR, absorption_coeff_cond);

    material_counter++;

    materials[1] = Material(1.0f, 0.8f, water_IOR, absorption_coeff_diel);

    material_counter++;

    //create shader programs and use the standard shader
    create_shader_programs();

    //after creati gprograms one can init render passes
    omni_shadow_map_pass = OmniShadowMapPass(omni_dir_shadow_shader_program);
    directional_shadow_map_pass = DirectionalShadowMapPass(shadow_map_shader_program);
    geometry_pass = GeometryPass(g_buffer_geometry_pass_shader_program);
    //lighting_pass = LightingPass();
    lighting_pass.init(g_buffer_lighting_pass_shader_program);
    //textures
    //ornament1 = Texture(_strdup("Textures/dirt.png"), new RepeatMode());
    //ornament1.load_texture_with_alpha_channel();

    tGenerator->init();

    //precompute our noise textures ones
    create_noise_textures();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(main_window->get_window(), true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);

    //enable depth testing
    glEnable(GL_DEPTH_TEST);

    //init texture for loading screen
    loading_screen.init();
    loading_screen_tex = Texture("Textures/Loading_Screen/SpaceX_rocket_with_logo.png", new RepeatMode());
    loading_screen_tex.load_texture_with_alpha_channel();
    logo = Texture("Textures/Loading_Screen/logo.png", new RepeatMode());
    logo.load_texture_with_alpha_channel();

    //scene.load_models();
    std::thread load_scene{ [] {

        scene.load_models();

    } };

    load_scene.detach();

    //

    while (!main_window->get_should_close()) {

        glViewport(0, 0, window_width, window_height);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat ratio = main_window->get_buffer_width() / main_window->get_buffer_height();
        //we need the projection matrix, just use glm::perspective function
        glm::mat4 projection_matrix = glm::perspective(glm::radians(main_camera->get_fov()),
            ratio, main_camera->get_near_plane(), main_camera->get_far_plane());

        //we should make the application independet of processor speed :)
        // take time into account is crucial
        // concept of delta time: https://bell0bytes.eu/keeping-track-of-time/
        GLfloat now = (float)glfwGetTime();
        delta_time = now - last_time;
        last_time = now;

        //poll all events incoming from user
        glfwPollEvents();

        // handle events for the camera
        main_camera->key_control(main_window->get_keys(), delta_time);
        main_camera->mouse_control(main_window->get_x_change(), main_window->get_y_change());

        //update light positions

        // feed inputs to dear imgui, start new frame
        //UI.start_new_frame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (scene.is_loaded()) {

            if (!loading_screen_finished) {

                loading_screen_finished = true;

            }

            scene.setup_game_object_context();

            point_lights[0].set_position(scene.get_position_of_current_ship() + glm::vec3(0.0, -24.f, -24.0));
            point_lights[1].set_position(scene.get_position_of_current_ship() + glm::vec3(15.0, 0.f, 0.0f));
            point_lights[2].set_position(scene.get_position_of_current_ship() + glm::vec3(-15.0, 0.f, 0.0));

            //retreive shadow map before our geometry pass
            main_light.calc_orthogonal_projections(main_camera->calculate_viewmatrix(),
                main_window->get_buffer_width(), main_window->get_buffer_height(), fov, num_shadow_cascades);

            directional_shadow_map_pass.execute(&main_light, main_camera->calculate_viewmatrix(),
                first_person_mode, &scene);

            // omni shadow map passes for our point lights
            for (size_t p_light_count = 0; p_light_count < point_light_count; p_light_count++) {
                omni_shadow_map_pass.execute(&point_lights[p_light_count], first_person_mode, &scene);
            }

            //we will now start the geometry pass 
            geometry_pass.execute(projection_matrix, main_camera->calculate_viewmatrix(), window_width, window_height, gbuffer.get_id(),
                first_person_mode, delta_time, &scene);

            // render the AABB for the clouds
            clouds->render(projection_matrix, main_camera->calculate_viewmatrix(), window_width, window_height);

            // after geometry pass we can now do the lighting

            lighting_pass.execute(projection_matrix, main_camera->calculate_viewmatrix(), &gbuffer, &main_light,
                point_lights, point_light_count, main_camera->get_camera_position(),
                material_counter, materials, &noise, clouds, delta_time);

        }
        else {

            // play the audio 
            //SoundEngine->play2D("Audio/Red_Dead_Redemption_2 _Loading_Screen.mp3", true); // 

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            loading_screen_tex.use_texture();

            loading_screen_shader_program->use_shader_program();

            loading_screen.render();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

        }

        // render your GUI
        ImGui::Begin("GUI v1.3.1");

        if (!scene.is_loaded())
        {
            ImGui::ProgressBar(scene.get_progress(), ImVec2(0.0f, 0.0f));
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("Load Planet");
            ImGui::Separator();
        }



        if (ImGui::CollapsingHeader("Hot shader reload")) {

            if (ImGui::Button("Hot reload ALL shaders!")) {
                reload_shader_programs();
            }

        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Procedural Generation")) {

            ImGui::InputInt("Height", &terrain_height);

            if (ImGui::Button("Adapt changes")) {
                tGenerator->changeMaxHeight((float)terrain_height);
            }

        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Graphic Settings")) {

            if (ImGui::TreeNode("Directional Light")) {
                ImGui::Separator();
                ImGui::SliderFloat("Ambient intensity", &direcional_light_ambient_intensity, 0.0f, 50.0f);
                ImGui::Separator();
                // Edit a color (stored as ~4 floats)
                ImGui::ColorEdit3("Directional Light Color", directional_light_color);
                ImGui::Separator();
                ImGui::SliderFloat3("Light Direction", directional_light_direction, -1.f, 1.0f);
                
                if (ImGui::TreeNode("Shadows")) {
                    int shadow_map_res_index_before = shadow_map_res_index;
                    ImGui::Combo("Shadow Map Resolution", &shadow_map_res_index, available_shadow_map_resolutions, IM_ARRAYSIZE(available_shadow_map_resolutions));
                    if (shadow_map_res_index_before != shadow_map_res_index) shadow_resolution_changed = true;
                    int num_cascades_before = num_shadow_cascades;
                    ImGui::SliderInt("# cascades", &num_shadow_cascades, NUM_MIN_CASCADES, NUM_MAX_CASCADES);
                    if (num_cascades_before != num_shadow_cascades) shadow_resolution_changed = true;
                    ImGui::SliderInt("PCF radius", &pcf_radius, 1, 20 );
                    ImGui::SliderFloat("Shadow intensity", &cascaded_shadow_intensity ,0.0f, 1.0f);
                    
                    ImGui::TreePop();

                }

                ImGui::TreePop();
            }


            if (ImGui::TreeNode("Cloud Settings")) {

                ImGui::SliderInt("Speed", &cloud_speed, 0, 15);
                ImGui::SliderFloat3("Movement Direction", cloud_movement_direction, -10.f, 10.0f);
                ImGui::SliderFloat("Illumination intensity", &cloud_scale, 0.f, 1.0f);
                ImGui::SliderFloat("Density", &cloud_density, 0.f, 1.0f);
                ImGui::SliderFloat("Pillowness", &cloud_pillowness, 0.f, 1.0f);
                ImGui::SliderFloat("Cirrus effect", &cloud_cirrus_effect, 0.f, 1.0f);
                ImGui::Checkbox("Powder effect", &cloud_powder_effect);
                ImGui::SliderFloat3("Scale", cloud_mesh_scale, 0.f, 1000.0f);

                if (ImGui::Button("Update Cloud params")) {
                    reload_noise_programs();
                }
            }

        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Control Settings")) {

            ImGui::Checkbox("Control Space Ship", &first_person_mode);
            ImGui::SliderInt("Choosen space ship", &choosen_space_ship, 1, 4);
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Audio Settings")) {

            ImGui::SliderFloat("Volume", &sound_volume, 0.0f, 1.0f);
           
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("GUI Settings")) {

            if (ImGui::SliderFloat("Frame Rounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f")) {
                style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
            }
            { bool border = (style.FrameBorderSize > 0.0f);  if (ImGui::Checkbox("FrameBorder", &border)) { style.FrameBorderSize = border ? 1.0f : 0.0f; } }
            ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("KEY Bindings")) {

            ImGui::Text("WASD for moving Forward, backward and to the side\n QE for rotating ");

        }

        ImGui::Separator();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //ImGui::ShowDemoWindow();

        ImGui::Image((void*)(intptr_t)logo.get_id(), ImVec2(500,100), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();

        // feed inputs to dear imgui, start new frame
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //update all vaiables
        //update light parameters
        main_light.set_ambient_intensity(direcional_light_ambient_intensity);
        main_light.set_diffuse_intensity(direcional_light_diffuse_intensity);
        main_light.get_shadow_map()->set_intensity(cascaded_shadow_intensity);

        glm::vec3 new_main_light_color(directional_light_color[0], directional_light_color[1], directional_light_color[2]);
        main_light.set_color(new_main_light_color);

        glm::vec3 new_main_light_pos(directional_light_direction[0], directional_light_direction[1], directional_light_direction[2]);
        main_light.set_direction(new_main_light_pos);

        if (scene.is_loaded()) scene.update_current_space_ship(choosen_space_ship);

        main_window->update_viewport();
        GLfloat new_window_width = main_window->get_buffer_width();
        GLfloat new_window_height = main_window->get_buffer_height();

        if ((new_window_width == window_width && window_height == window_height) == false) {

            window_height = new_window_height;
            window_width = new_window_width;;
            gbuffer.update_window_params(window_width, window_height);
            gbuffer.create();
            clouds->update_window_params(window_width, window_height);

        }

        glm::vec3 cloud_move;
        cloud_move.x = cloud_movement_direction[0];
        cloud_move.y = cloud_movement_direction[1];
        cloud_move.x = cloud_movement_direction[2];

        clouds->set_movement_direction(cloud_move);
        clouds->set_movement_speed(cloud_speed);
        clouds->set_density(cloud_density);
        clouds->set_scale(cloud_scale);
        clouds->set_pillowness(cloud_pillowness);
        clouds->set_cirrus_effect(cloud_cirrus_effect);
        clouds->set_powder_effect(cloud_powder_effect);
        clouds->set_scale(glm::vec3(cloud_mesh_scale[0], cloud_mesh_scale[1], cloud_mesh_scale[2]));
        main_light.get_shadow_map()->set_pcf_radius(pcf_radius);

        if (loading_screen_finished) {
            
        }
        if (shadow_resolution_changed) {

                switch (shadow_map_res_index) {
                case 0:
                    shadow_map_resolution = 512.f;
                    break;
                case 1:
                    shadow_map_resolution = 1024.f;
                    break;
                case 2:
                    shadow_map_resolution = 2048.f;
                    break;
                case 3:
                    shadow_map_resolution = 4096.f;
                }

                main_light.update_shadow_map(shadow_map_resolution, shadow_map_resolution, num_shadow_cascades);

                shadow_resolution_changed = false;
        }

        main_window->swap_buffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

}


