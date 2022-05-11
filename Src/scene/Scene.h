#pragma once
#include <vector>
#include <mutex>
#include <thread>

#include "GameObject.h"
#include "ViewFrustumCulling.h"
#include "RenderPassSceneDependend.h"
#include "Window.h"
#include "Clouds.h"
#include "ObjMaterial.h"
#include "PointLight.h"
#include "Rotation.h"
#include "DirectionalLight.h"

class Scene
{

public:

	Scene();

	Scene(const Scene& other);

	std::thread spwan() {
		return std::thread([=] {load_models();});
	}

	void init(	std::shared_ptr<Camera> main_camera, 
				std::shared_ptr<Window> main_window);

	void render(RenderPassSceneDependend* render_pass);

	GLuint										get_point_light_count();
	std::shared_ptr<DirectionalLight>			get_sun();
	std::vector<std::shared_ptr<PointLight>>	get_point_lights();
	std::vector<ObjMaterial>					get_materials();
	GLfloat										get_progress();
	int											get_texture_count(int index);
	bool										get_context_setup();
	std::shared_ptr<Clouds>						get_clouds();

	void add_ambient_object(std::string model_path, glm::vec3 translation, 
							GLfloat scale, Rotation rot);
	void load_models();

	bool is_loaded();
	void setup_game_object_context();

	void bind_textures_and_buffer();
	void unbind_textures_and_buffer();

	void set_context_setup(bool context_setup);

	
	~Scene();

private:

	bool object_is_visible(std::shared_ptr<GameObject> game_object);

	std::shared_ptr<Camera>						main_camera;

	//lights
	std::shared_ptr<DirectionalLight>			sun;
	std::vector<std::shared_ptr<PointLight>>	point_lights;
	std::shared_ptr<Clouds>						clouds;

	std::shared_ptr<Window>						main_window;
	std::shared_ptr<ViewFrustumCulling>			view_frustum_culling;

	std::vector<std::shared_ptr<GameObject>>	ambient_objects;

	GLfloat		progress;
	bool		loaded_scene;

	std::mutex	mx_progress;
	std::mutex	mx_isLoaded;
	std::mutex	mx_space_ship;

	bool		context_setup;

};

