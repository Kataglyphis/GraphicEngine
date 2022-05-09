#include "Renderer.h"

Renderer::Renderer(GLuint window_width, GLuint window_height)
{

    this->window_width  = window_width;
    this->window_height = window_height;

    gbuffer = std::make_shared<GBuffer>(window_width, window_height);
    gbuffer->create();

    //after creating programs one can init render passes
    omni_shadow_map_pass        = OmniShadowMapPass();
    directional_shadow_map_pass = DirectionalShadowMapPass();
    geometry_pass               = GeometryPass();
    lighting_pass               = LightingPass();
    lighting_pass.init();
}

void Renderer::drawFrame(   std::shared_ptr<Camera> main_camera,
                            std::shared_ptr<Scene> scene,
                            glm::mat4 projection_matrix,
                            GLfloat delta_time)
{

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
    std::shared_ptr<Clouds> clouds = scene->get_clouds();
    clouds->render( projection_matrix, main_camera->calculate_viewmatrix(),
                    window_width, window_height);

    // after geometry pass we can now do the lighting
    lighting_pass.execute(  projection_matrix,
                            main_camera,
                            scene,
                            gbuffer,
                            delta_time);

}

void Renderer::update_window_params(GLuint window_width, GLuint window_height)
{
    this->window_width = window_width;
    this->window_height = window_height;
    gbuffer->update_window_params(window_width, window_height);
    gbuffer->create();
}

void Renderer::reload_shader_programs()
{
    //set_shader_includes();
    /*shadow_map_shader_program->reload();
    g_buffer_geometry_pass_shader_program->reload();
    g_buffer_lighting_pass_shader_program->reload();
    omni_dir_shadow_shader_program->reload();
    clouds->get_shader_program()->reload();*/

    /*noise->update();
    create_noise_textures();*/
}

Renderer::~Renderer()
{
}
