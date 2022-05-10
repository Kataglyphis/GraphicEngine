#pragma once
class RenderPass
{
public:

	RenderPass();

	virtual void create_shader_program() = 0;

	~RenderPass();

private:

};

