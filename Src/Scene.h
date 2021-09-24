#pragma once
#include <vector>
#include "GameObject.h"
#include "ViewFrustumCulling.h"
#include "RenderPassSceneDependend.h"
#include "MyWindow.h"
#include <mutex>
#include "Terrain_generator.h"
#include "Clouds.h"
#include <thread>

class Scene
{

public:

	Scene();

	Scene(const Scene& other);

	std::shared_ptr<Terrain_Generator> get_terrain_generator();

	std::thread spwan() {
		return std::thread([=] {load_models();});
	}

	void init(std::shared_ptr<Camera> main_camera, MyWindow* main_window, std::shared_ptr<Terrain_Generator> terrain_generator, std::shared_ptr<Clouds> clouds);
	void render(RenderPassSceneDependend* render_pass, bool first_person_mode);
	void add_space_ship(std::string model_path, glm::vec3 translation, GLfloat scale, Rotation rot, GLuint material_id);
	void add_ambient_object(std::string model_path, glm::vec3 translation, GLfloat scale, Rotation rot, GLuint material_id);
	void update_current_space_ship(GLuint selected_space_ship);
	void load_models();
	bool is_loaded();
	GLfloat get_progress();
	void setup_game_object_context();

	void set_context_setup(bool context_setup);
	bool get_context_setup();

	glm::vec3 get_position_of_current_ship();
	std::shared_ptr<Clouds> get_clouds();
	
	~Scene();

private:

	bool object_is_visible(std::shared_ptr<GameObject> game_object);

	std::shared_ptr<Camera> main_camera;
	MyWindow* main_window;
	std::shared_ptr<Terrain_Generator> terrain_generator;
	std::shared_ptr<ViewFrustumCulling> view_frustum_culling;
	std::shared_ptr<Clouds> clouds;

	std::vector<std::shared_ptr<GameObject>> space_ships;
	std::vector<std::shared_ptr<GameObject>> ambient_objects;

	std::vector<glm::vec3> space_ship_offsets;
	std::vector<GLfloat> rotation_offset;

	GLfloat progress;
	bool loaded_scene;

	std::mutex mx_progress;
	std::mutex mx_isLoaded;
	std::mutex mx_space_ship;

	GLuint current_space_ship_selected;

	bool context_setup;

};

