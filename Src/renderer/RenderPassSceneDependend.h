#pragma once
#include <vector>

#include "Model.h"
#include "RenderPass.h"

class RenderPassSceneDependend : public RenderPass
{
public:

	RenderPassSceneDependend();

	virtual void set_game_object_uniforms(glm::mat4 model, glm::mat4 normal_model) = 0;
	virtual void create_shader_program() = 0;

	~RenderPassSceneDependend();

private:

};

