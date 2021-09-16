#pragma once
#include <vector>
#include "GameObject.h"
#include "ViewFrustumCulling.h"
#include "RenderPassSceneDependend.h"
#include "MyWindow.h"
#include <mutex>
#include "Terrain_generator.h"
#include "Clouds.h"

class Scene
{

public:

	Scene();

	Terrain_Generator* get_terrain_generator();

	void init(Camera* main_camera, MyWindow* main_window, Terrain_Generator* terrain_generator, Clouds* clouds);
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
	Clouds* get_clouds();
	
	~Scene();

private:

	bool object_is_visible(GameObject* game_object);

	Camera* main_camera;
	MyWindow* main_window;
	Terrain_Generator* terrain_generator;
	ViewFrustumCulling* view_frustum_culling;
	Clouds* clouds;

	std::vector<GameObject*> space_ships;
	std::vector<GameObject*> ambient_objects;

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

