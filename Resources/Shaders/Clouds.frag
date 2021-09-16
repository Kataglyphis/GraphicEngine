#version 330

layout(location = 0) out vec4 cloud;

in float frag_depth;
in vec3 cloud_pos;

void main() {
	
	cloud = vec4(cloud_pos, frag_depth);

}