#pragma once
#include "Model.h"

class GameObject
{
public:

	GameObject();

	void init(std::string model_path, glm::vec3 translation, GLfloat scale, Rotation rot, GLuint material_id);

	void set_material_id(GLuint material_id);

	glm::mat4 get_world_trafo();
	glm::mat4 get_normal_world_trafo();
	GLuint get_material_id();
	AABB* get_aabb();
	Model* get_model();

	void translate(glm::vec3 translate);
	void scale(GLfloat scale_factor);
	void rotate(Rotation rot);

	void render();

	~GameObject();


private:

	GLuint material_id;
	Model* model;
	
	GLfloat scale_factor;
	Rotation rot;
	glm::vec3 translation;

};

