#pragma once
#include <vector>
#include "GameObject.h"
#include "ViewFrustumCulling.h"
#include "RenderPassSceneDependend.h"
#include "Window.h"
#include <mutex>
#include "Clouds.h"
#include <thread>
#include <ObjMaterial.h>

class Scene
{

public:

	Scene();

	Scene(const Scene& other);

	std::thread spwan() {
		return std::thread([=] {load_models();});
	}

	void init(std::shared_ptr<Camera> main_camera, Window* main_window, std::shared_ptr<Clouds> clouds);
	void render(RenderPassSceneDependend* render_pass, bool first_person_mode);
	void add_ambient_object(std::string model_path, glm::vec3 translation, GLfloat scale, Rotation rot);
	void load_models();

	std::vector<ObjMaterial> get_materials();
	bool is_loaded();
	GLfloat get_progress();
	void setup_game_object_context();

	void bind_textures_and_buffer();
	void unbind_textures_and_buffer();

	int get_texture_count(int index);

	void set_context_setup(bool context_setup);
	bool get_context_setup();

	std::shared_ptr<Clouds> get_clouds();
	
	~Scene();

private:

	bool object_is_visible(std::shared_ptr<GameObject> game_object);

	std::shared_ptr<Camera> main_camera;
	Window* main_window;
	std::shared_ptr<ViewFrustumCulling> view_frustum_culling;
	std::shared_ptr<Clouds> clouds;

	std::vector<std::shared_ptr<GameObject>> ambient_objects;

	GLfloat progress;
	bool loaded_scene;

	std::mutex mx_progress;
	std::mutex mx_isLoaded;
	std::mutex mx_space_ship;

	bool context_setup;

};

