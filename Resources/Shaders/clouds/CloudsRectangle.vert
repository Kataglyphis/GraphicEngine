#version 460

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float frag_depth;
out vec3 cloud_pos;

void main() {
	
	cloud_pos = vec3(model * vec4(pos, 1.0f));
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	frag_depth = gl_Position.z;
}