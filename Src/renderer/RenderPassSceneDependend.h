#pragma once
#include <vector>
//#include <GL\glew.h>
#include "Model.h"

class RenderPassSceneDependend
{
public:

	RenderPassSceneDependend();

	virtual void set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model) = 0;

	~RenderPassSceneDependend();

private:

};

