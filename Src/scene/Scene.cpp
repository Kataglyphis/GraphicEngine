#include "Scene.h"

Scene::Scene()
{

}

Scene::Scene(const Scene& other)
{
    main_camera = other.main_camera;
    main_window = other.main_window;
    terrain_generator = other.terrain_generator;
    view_frustum_culling = other.view_frustum_culling;
    clouds = other.clouds;


    ambient_objects = other.ambient_objects;

     progress = other.progress;
     loaded_scene = other.loaded_scene;

     context_setup = other.context_setup;
}

std::shared_ptr<Terrain_Generator> Scene::get_terrain_generator()
{
    return terrain_generator;
}

void Scene::init(std::shared_ptr<Camera> main_camera, MyWindow* main_window, std::shared_ptr<Terrain_Generator> terrain_generator, std::shared_ptr<Clouds> clouds)
{

    this->main_camera = main_camera;
    this->main_window = main_window;
    this->terrain_generator = terrain_generator;
    this->clouds = clouds;

    loaded_scene = false;

    view_frustum_culling = std::make_shared<ViewFrustumCulling>(ViewFrustumCulling{});

    progress = 0;

    context_setup = false;

}

void Scene::load_models()
{

    lock_guard<mutex> guard{ mx_space_ship };

    glm::vec3 space_station_offset = glm::vec3(-130.f, 0.0f, 50.0f);
    GLfloat space_station_scale = 0.4f;
    Rotation space_station_rot;
    space_station_rot.degrees = 0.0f;
    space_station_rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 clouds_offset = glm::vec3(30.f, 400.0f, 30.0f);
    glm::vec3 clouds_scale = glm::vec3(64.f, 64.f, 64.f);
    clouds->set_scale(clouds_scale);
    clouds->set_translation(clouds_offset);

    std::shared_ptr<GameObject> sponza = make_shared<GameObject>("Models/crytek-sponza/sponza_triag.obj", space_station_offset, space_station_scale, space_station_rot, 0);
    progress += 1.f;

    ambient_objects.push_back(sponza);

    mx_isLoaded.lock();
    loaded_scene = true;
    mx_isLoaded.unlock();

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
    
    for (std::shared_ptr<GameObject>& ambient_object : ambient_objects) {

        ambient_object->get_model()->create_render_context();

    }

    //terrain_generator->generate_render_context();

    context_setup = true;
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
                                                    object->get_normal_world_trafo(),
                                                    object->get_material_id());

            object->render();
        //}

    }

    /*render_pass->set_game_object_uniforms(terrain_generator->get_world_trafo(),
                                                                        terrain_generator->get_normal_world_trafo(),
                                                                        terrain_generator->get_material_id());*/

    //std::vector<bool> terrainRenderedFlags;
    //if (render_pass->use_terrain_textures()) terrain_generator->get_textures()->use_texture();
    
    //terrainRenderedFlags = terrain_generator->render(main_window->get_buffer_width() / main_window->get_buffer_height(),
                                                //main_camera, view_frustum_culling, render_pass);


    //if (render_pass->use_terrain_textures()) terrain_generator->get_textures()->unbind_texture();


    //terrain_generator->render_plants(terrainRenderedFlags, render_pass);
    
    
}

void Scene::add_ambient_object(std::string model_path, glm::vec3 translation, GLfloat scale, Rotation rot, GLuint material_id)
{
    ambient_objects.push_back(std::make_shared<GameObject>(GameObject()));
    ambient_objects.back()->init(model_path, translation, scale, rot, material_id);
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

