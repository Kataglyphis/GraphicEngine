#pragma once
#include <vector>
//#include <GL\glew.h>
#include "Model.h"
//
//#include "Terrain_generator.h"
//#include "Camera.h"
//#include "Clouds.h"

class RenderPassSceneDependend
{
public:

	RenderPassSceneDependend();

	virtual void set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model) = 0;

	virtual bool use_terrain_textures() = 0;

	~RenderPassSceneDependend();

private:

};

