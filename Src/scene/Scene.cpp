#include "Scene.h"

Scene::Scene()
{

}

Scene::Scene(const Scene& other)
{
    main_camera = other.main_camera;
    main_window = other.main_window;
    view_frustum_culling = other.view_frustum_culling;
    clouds = other.clouds;


    ambient_objects = other.ambient_objects;

    progress = other.progress;
    loaded_scene = other.loaded_scene;

    context_setup = other.context_setup;
}

void Scene::init(std::shared_ptr<Camera> main_camera, MyWindow* main_window, std::shared_ptr<Clouds> clouds)
{

    this->main_camera = main_camera;
    this->main_window = main_window;
    this->clouds = clouds;

    loaded_scene = false;

    view_frustum_culling = std::make_shared<ViewFrustumCulling>(ViewFrustumCulling{});

    progress = 0;

    context_setup = false;

}

void Scene::load_models()
{

    lock_guard<mutex> guard{ mx_space_ship };

    glm::vec3 sponza_offset = glm::vec3(0.f, 0.0f, 0.0f);
    GLfloat sponza_scale = 10.f;
    Rotation sponza_rot;
    sponza_rot.degrees = 0.0f;
    sponza_rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 clouds_offset = glm::vec3(30.f, 800.0f, 30.0f);
    glm::vec3 clouds_scale = glm::vec3(64.f, 64.f, 64.f);
    clouds->set_scale(clouds_scale);
    clouds->set_translation(clouds_offset);

    std::shared_ptr<GameObject> sponza = make_shared<GameObject>(   /*"../Resources/Models/Pillum/PilumPainting_Export.obj",*/
                                                                    "../Resources/Models/dinosaurs.obj",
                                                                    /*"../Resources/Models/crytek-sponza/sponza_triag.obj"*/
                                                                    sponza_offset, sponza_scale,
                                                                    sponza_rot);
    progress += 1.f;

    ambient_objects.push_back(sponza);

    mx_isLoaded.lock();
    loaded_scene = true;
    mx_isLoaded.unlock();

}

std::vector<ObjMaterial> Scene::get_materials()
{
    return ambient_objects[0]->get_model()->get_materials();
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
    ambient_objects[0]->get_model()->create_render_context();
    context_setup = true;
}

void Scene::bind_textures_and_buffer()
{
    ambient_objects[0]->get_model()->bind_ressources();
}

void Scene::unbind_textures_and_buffer()
{
    ambient_objects[0]->get_model()->unbind_resources();
}

int Scene::get_texture_count(int index)
{
    return ambient_objects[0]->get_model()->get_texture_count();
}

void Scene::set_context_setup(bool context_setup)
{
    this->context_setup = context_setup;
}

bool Scene::get_context_setup()
{
    return context_setup;
}

void Scene::render(RenderPassSceneDependend* render_pass, bool first_person_mode)
{
    lock_guard<mutex> guard{ mx_space_ship };
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //aabb->render();
     //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     // 
    // update model matrix accordingly to our camera position

    /*for (GameObject* space_ship : space_ships) {
        
        if (object_is_visible(space_ship)) {

            render_pass->set_game_object_uniforms(space_ship->get_world_trafo(), 
                                                                                space_ship->get_normal_world_trafo(), 
                                                                                space_ship->get_material_id());
            space_ship->render();
        }

    }*/

    //GameObject* current_space_ship = space_ships[current_space_ship_selected];

    for (std::shared_ptr<GameObject> object : ambient_objects) {

       /* if (object_is_visible(object)) {*/

            render_pass->set_game_object_uniforms(  object->get_world_trafo(),
                                                    object->get_normal_world_trafo());

            object->render();
        //}

    }
    
}

void Scene::add_ambient_object(std::string model_path, glm::vec3 translation, GLfloat scale, Rotation rot)
{
    ambient_objects.push_back(std::make_shared<GameObject>(GameObject()));
    ambient_objects.back()->init(model_path, translation, scale, rot);
}

std::shared_ptr<Clouds> Scene::get_clouds()
{
    return clouds;
}

bool Scene::object_is_visible(std::shared_ptr<GameObject> game_object)
{
    return view_frustum_culling->is_inside(main_window->get_buffer_width()/main_window->get_buffer_height(),
                                                            main_camera, game_object->get_aabb(), game_object->get_world_trafo());
}

Scene::~Scene()
{
}

