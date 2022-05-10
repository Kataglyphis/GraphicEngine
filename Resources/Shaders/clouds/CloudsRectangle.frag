#version 460

layout(location = 0) out vec3 g_position;
layout(location = 3) out vec3 g_material_id;

in vec4 cloud_world_pos;

uniform int cloudsMaterialID;

void main() {
	
	g_position		= cloud_world_pos.xyz;
	g_material_id	= vec3(cloudsMaterialID);

}