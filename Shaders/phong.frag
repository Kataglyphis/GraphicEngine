#version 330 

in vec2 tex_coord;
in vec3 normal;

out vec4 color;

struct Light {

    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;

};

struct DirectionalLight {
    
    Light base;
    vec3 direction;

};

struct Material {

    float specular_intensity;
    float shininess;

};

uniform sampler2D brick_texture;
uniform DirectionalLight directional_light;

void main() {
	
	float kd = directional_light.base.diffuse_intensity * max(0.0f, dot(normalize(directional_light.direction),normalize(normal)));
	color += texture(brick_texture, tex_coord); //vec4(1.0f, 0.0f, 0.0f, 1.0f);
    color += kd *vec4(directional_light.base.color, 1.0) * directional_light.base.ambient_intensity;
	//color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}