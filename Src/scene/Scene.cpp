#include "Scene.h"

#include <sstream>

Scene::Scene()
{

}

Scene::Scene(const Scene& other)
{
    main_camera = other.main_camera;
    main_window = other.main_window;
    view_frustum_culling = other.view_frustum_culling;
    clouds = other.clouds;


    game_objects = other.game_objects;

    progress = other.progress;
    loaded_scene = other.loaded_scene;

    context_setup = other.context_setup;
}

void Scene::init(std::shared_ptr<Camera> main_camera, std::shared_ptr<Window> main_window)
{

    this->main_camera = main_camera;
    this->main_window = main_window;

    //global objects
    sun = std::make_shared<DirectionalLight>(   4096,
                                                4096,
                                                1.f,
                                                1.f,
                                                1.f,
                                                1.f,
                                                -0.1f,
                                                -0.8,
                                                -0.1f,
                                                main_camera->get_near_plane(), main_camera->get_far_plane(),
                                                NUM_CASCADES);

    point_lights.reserve(MAX_POINT_LIGHTS);
    point_lights.push_back(std::make_shared<PointLight>( 1024, 1024,
                                                        0.01f, 100.f,
                                                        0.0f, 1.0f, 0.0f,
                                                        1.0f,
                                                        0.0f, 0.0f, 0.0f,
                                                        0.1f, 0.1f, 0.1f));

    point_lights[0]->set_position(glm::vec3(0.0, -24.f, -24.0));

    clouds = std::make_shared<Clouds>();

    loaded_scene = false;

    view_frustum_culling = std::make_shared<ViewFrustumCulling>(ViewFrustumCulling{});

    progress = 0;

    context_setup = false;

}

GLuint Scene::get_point_light_count()
{
    return static_cast<uint32_t>(point_lights.size());
}

std::shared_ptr<DirectionalLight> Scene::get_sun()
{
    return sun;
}

std::vector<std::shared_ptr<PointLight>> Scene::get_point_lights()
{
    return point_lights;
}

void Scene::load_models()
{

    lock_guard<mutex> guard{ mx_space_ship };

    glm::vec3 sponza_offset = glm::vec3(0.f, 0.0f, 0.0f);
    GLfloat sponza_scale = 10.f;
    Rotation sponza_rot;
    sponza_rot.degrees = 0.0f;
    sponza_rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 clouds_offset = glm::vec3(1.f, 80.0f, 1.0f);
    glm::vec3 clouds_scale = glm::vec3(1.f, 1.f, 1.f);
    clouds->set_scale(clouds_scale);
    clouds->set_translation(clouds_offset);

    std::stringstream modelFile;
    modelFile << CMAKELISTS_DIR << "/Resources/Models/dinosaurs.obj";
    /*"../Resources/Models/Pillum/PilumPainting_Export.obj",*/
    /*"../Resources/Models/crytek-sponza/sponza_triag.obj",*/

    std::shared_ptr<GameObject> sponza = make_shared<GameObject>(   modelFile.str(),
                                                                    sponza_offset, sponza_scale,
                                                                    sponza_rot);
    progress += 1.f;

    game_objects.push_back(sponza);

    mx_isLoaded.lock();
    loaded_scene = true;
    mx_isLoaded.unlock();

}

std::vector<ObjMaterial> Scene::get_materials()
{
    return game_objects[0]->get_model()->get_materials();
}

bool Scene::is_loaded()
{
    lock_guard<mutex> guard{mx_isLoaded};
    return loaded_scene;
}

GLfloat Scene::get_progress()
{
    lock_guard<mutex> guard{mx_progress};
    return progress;
}

void Scene::setup_game_object_context()
{
    game_objects[0]->get_model()->create_render_context();
    context_setup = true;
}

void Scene::bind_textures_and_buffer()
{
    game_objects[0]->get_model()->bind_ressources();
}

void Scene::unbind_textures_and_buffer()
{
    game_objects[0]->get_model()->unbind_resources();
}

int Scene::get_texture_count(int index)
{
    return game_objects[0]->get_model()->get_texture_count();
}

void Scene::set_context_setup(bool context_setup)
{
    this->context_setup = context_setup;
}

bool Scene::get_context_setup()
{
    return context_setup;
}

void Scene::add_game_object(std::string model_path, glm::vec3 translation, GLfloat scale, Rotation rot)
{
    game_objects.push_back(std::make_shared<GameObject>(GameObject()));
    game_objects.back()->init(model_path, translation, scale, rot);
}

std::shared_ptr<Clouds> Scene::get_clouds()
{
    return clouds;
}

std::vector<std::shared_ptr<GameObject>> Scene::get_game_objects()
{
    return game_objects;
}

bool Scene::object_is_visible(std::shared_ptr<GameObject> game_object)
{
    return view_frustum_culling->is_inside( main_window->get_buffer_width()/main_window->get_buffer_height(),
                                            main_camera, game_object->get_aabb(), game_object->get_world_trafo());
}

Scene::~Scene()
{
}

