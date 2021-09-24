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

    space_ships = other.space_ships;
    ambient_objects = other.ambient_objects;

     space_ship_offsets = other.space_ship_offsets;
     rotation_offset = other.rotation_offset;

     progress = other.progress;
     loaded_scene = other.loaded_scene;

     current_space_ship_selected = other.current_space_ship_selected;

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
    current_space_ship_selected = 0;

    progress = 0;

    context_setup = false;

}

void Scene::load_models()
{

    lock_guard<mutex> guard{ mx_space_ship };

    // = std::make_shared<GameObject>();
    // = std::make_shared<GameObject>();
    // = std::make_shared<GameObject>();
    // = std::make_shared<GameObject>();

    // = std::make_shared<GameObject>();
    // = std::make_shared<GameObject>();

    glm::vec3 space_ship1_offset = glm::vec3(0.0f, 25.0f, 0.0f);
    GLfloat space_ship1_scale = 10.0f;
    glm::vec3 space_ship2_offset = space_ship1_offset + glm::vec3(50.f, 15.0f, 0.0f);
    GLfloat space_ship2_scale = 0.4f;
    glm::vec3 space_ship3_offset = space_ship1_offset + glm::vec3(150.f, 15.0f, 0.0f);
    GLfloat space_ship3_scale = 10.f;
    glm::vec3 space_ship4_offset = space_ship1_offset + glm::vec3(-150.f, 45.0f, 0.0f);
    GLfloat space_ship4_scale = 5.f;

    glm::vec3 rocket_offset = space_ship1_offset + glm::vec3(-20.f, 0.0f, -20.0f);
    GLfloat rocket_scale = 0.5f;
    Rotation rocket_rot;
    rocket_rot.degrees = 0.0f;
    rocket_rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 space_station_offset = space_ship1_offset + glm::vec3(-130.f, 0.0f, 50.0f);
    GLfloat space_station_scale = 0.4f;
    Rotation space_station_rot;
    space_station_rot.degrees = 0.0f;
    space_station_rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 clouds_offset = space_ship1_offset + glm::vec3(30.f, 200.0f, 30.0f);
    glm::vec3 clouds_scale = glm::vec3(64.f, 64.f, 64.f);
    clouds->set_scale(clouds_scale);
    clouds->set_translation(clouds_offset);

    Rotation rot_space_ship_1;
    rot_space_ship_1.degrees = 180.f;
    rot_space_ship_1.axis = glm::vec3(0.0f, 1.0f, 0.0f);
    Rotation rot_space_ship_2;
    rot_space_ship_2.degrees = 0.0f;
    rot_space_ship_2.axis = glm::vec3(0.0f, 1.0f, 0.0f);
    Rotation rot_space_ship_3;
    rot_space_ship_3.degrees = -90.f;
    rot_space_ship_3.axis = glm::vec3(0.0f, 1.0f, 0.0f);
    Rotation rot_space_ship_4;
    rot_space_ship_4.degrees = 90.f;
    rot_space_ship_4.axis = glm::vec3(0.0f, 1.0f, 0.0f);

    std::shared_ptr<GameObject> space_ship_1 = make_shared<GameObject>("Models/Aircraft/E_45_Aircraft_obj.obj", space_ship1_offset, space_ship1_scale, rot_space_ship_1, 0);
    progress += 0.1f;
    std::shared_ptr<GameObject> space_ship_2 = make_shared<GameObject>("Models/sd85_cargo_starship/sd85_cargo_starship_flying.obj", space_ship2_offset, space_ship2_scale, rot_space_ship_2, 0);
    progress += 0.1f;
    std::shared_ptr<GameObject> space_ship_3 = make_shared<GameObject>("Models/SS1_OBJ/SS1.obj", space_ship3_offset, space_ship3_scale, rot_space_ship_3, 0);
    progress += 0.1f;
    std::shared_ptr<GameObject> space_ship_4 = make_shared<GameObject>("Models/Viper-mk-IV-fighter_obj/Viper-mk-IV-fighter.obj", space_ship4_offset, space_ship4_scale, rot_space_ship_4, 0);
    progress += 0.1f;
    std::shared_ptr<GameObject> rocket = make_shared<GameObject>("Models/Rocket/Rocket.obj", rocket_offset, rocket_scale, rocket_rot, 0);
    progress += 0.1f;
    std::shared_ptr<GameObject> space_station = make_shared<GameObject>("Models/helidrone-station/Helidrone_Station.obj", space_station_offset, space_station_scale, space_station_rot, 0);
    progress += 0.1f;
    terrain_generator->load_plants();
    progress += 0.1f;
    terrain_generator->generateMesh(progress);

    space_ships.push_back(space_ship_1);
    space_ships.push_back(space_ship_2);
    space_ships.push_back(space_ship_3);
    space_ships.push_back(space_ship_4);

    ambient_objects.push_back(space_station);
    ambient_objects.push_back(rocket);

    space_ship_offsets.push_back(glm::vec3(0.f, -24.f, 0.f));
    space_ship_offsets.push_back(glm::vec3(0.f, -50.f, 0.f));
    space_ship_offsets.push_back(glm::vec3(0.f, -60.f, 20.f));
    space_ship_offsets.push_back(glm::vec3(0.f, -30.f, 40.f));

    rotation_offset.push_back(90.f);
    rotation_offset.push_back(-90.f);
    rotation_offset.push_back(0.f);
    rotation_offset.push_back(-180.f);

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
    if (context_setup) return;
    for (std::shared_ptr<GameObject>& space_ship : space_ships) {

        space_ship->get_model()->create_render_context();

    }

    for (std::shared_ptr<GameObject>& ambient_object : ambient_objects) {

        ambient_object->get_model()->create_render_context();

    }

    terrain_generator->generate_render_context();

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

    if (first_person_mode) {

        glm::vec3 offset = space_ship_offsets[current_space_ship_selected];//glm::vec3(0.0, -4.0, -6.0);
        space_ships[current_space_ship_selected]->translate(main_camera->get_camera_position() + offset);
        Rotation rot;
        rot.axis = glm::vec3(0.0f,1.0f,0.0f);
        rot.degrees = -(main_camera->get_yaw() + rotation_offset[current_space_ship_selected]);
        space_ships[current_space_ship_selected]->rotate(rot);
    }

    /*for (GameObject* space_ship : space_ships) {
        
        if (object_is_visible(space_ship)) {

            render_pass->set_game_object_uniforms(space_ship->get_world_trafo(), 
                                                                                space_ship->get_normal_world_trafo(), 
                                                                                space_ship->get_material_id());
            space_ship->render();
        }

    }*/

    //GameObject* current_space_ship = space_ships[current_space_ship_selected];

    render_pass->set_game_object_uniforms(space_ships[current_space_ship_selected]->get_world_trafo(),
        space_ships[current_space_ship_selected]->get_normal_world_trafo(),
        space_ships[current_space_ship_selected]->get_material_id());
    space_ships[current_space_ship_selected]->render();

    for (std::shared_ptr<GameObject> object : ambient_objects) {

       /* if (object_is_visible(object)) {*/

            render_pass->set_game_object_uniforms(object->get_world_trafo(),
                                                                                object->get_normal_world_trafo(),
                                                                                object->get_material_id());

            object->render();
        //}

    }

    render_pass->set_game_object_uniforms(terrain_generator->get_world_trafo(),
                                                                        terrain_generator->get_normal_world_trafo(),
                                                                        terrain_generator->get_material_id());

    std::vector<bool> terrainRenderedFlags;
    if (render_pass->use_terrain_textures()) terrain_generator->get_textures()->use_texture();
    
    terrainRenderedFlags = terrain_generator->render(main_window->get_buffer_width() / main_window->get_buffer_height(),
                                                main_camera, view_frustum_culling, render_pass);


    if (render_pass->use_terrain_textures()) terrain_generator->get_textures()->unbind_texture();


    terrain_generator->render_plants(terrainRenderedFlags, render_pass);
    
    
}

void Scene::add_space_ship(std::string model_path, glm::vec3 translation, GLfloat scale, Rotation rot, GLuint material_id)
{

    space_ships.push_back(std::make_shared<GameObject>(GameObject()));
    space_ships.back()->init(model_path, translation, scale, rot, material_id);
}

void Scene::add_ambient_object(std::string model_path, glm::vec3 translation, GLfloat scale, Rotation rot, GLuint material_id)
{
    ambient_objects.push_back(std::make_shared<GameObject>(GameObject()));
    ambient_objects.back()->init(model_path, translation, scale, rot, material_id);
}

void Scene::update_current_space_ship(GLuint selected_space_ship)
{
    current_space_ship_selected = selected_space_ship - 1;
}

glm::vec3 Scene::get_position_of_current_ship()
{
    return glm::vec3(space_ships[current_space_ship_selected]->get_world_trafo()[3]);
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

