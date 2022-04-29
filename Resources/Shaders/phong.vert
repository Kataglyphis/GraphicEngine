#version 330 

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex_coord;

out vec2 tex_coord;
out vec3 normal;
in vec3 worldPosition;

//uniform variables
uniform mat4 projection;                
uniform mat4 view;                                                            
uniform mat4 model;
uniform mat4 normal_model;

void main() {
	
	gl_Position = projection * view * model * vec4(in_position, 1.0f);
	tex_coord = in_tex_coord;

	vec3 worldPosition = vec3(model * vec4(in_position, 1.0f));
	normal = mat3(transpose(inverse(model))) * in_normal;

}