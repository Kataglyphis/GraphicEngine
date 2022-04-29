#version 330

float PI = 3.14159265358979323846;

in vec2 tex_coord;
in vec3 normal;
in vec3 worldPosition;

out vec4 color;

float CosTheta(const vec3 w, vec3 N) { 
    return dot(w,N); 
}

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

uniform vec3 cam_pos;

vec3 LambertDiffuse(vec3 ambient) {

    return ambient/PI;

}

void main() {
	
	vec3 ambient    = texture(brick_texture, tex_coord);
    vec3 L          = normalize(-directional_light.direction);
    vec3 N          = normalize(normal);
    vec3 V          = normalize(cam_pos - worldPosition);

    // ignore ambient term for now :)
    float ka = 0.f;
    float ks = 0.3f;

    // exponent controls roughness; is in [0,1] because of remapping
    float n = 0.5;
    // use remapping for better user controllability
    // use the same as in Call of Duty: Black Ops (source RealTimeRenderingThirdEdition page 340)
    float maximumSpecular = 8192;
    float remappedN = pow(maximumSpecular, n);

    // add lambertian diffuse term
    vec3 color = LambertDiffuse(ambient);
    vec3 wh = normalize(L + V);

    // add specular term  
    if (CosTheta(L, N) > 0 && CosTheta(V, N) > 0) {
        color += directional_light.base.color * ks * ((remappedN + 8.f)/(8.f*PI) * pow(CosTheta(wh,), remappedN));
    }

    return color;

	//float kd = directional_light.base.diffuse_intensity * max(0.0f, dot(normalize(directional_light.direction),normalize(normal)));
    color += kd *vec4(directional_light.base.color, 1.0) * directional_light.base.ambient_intensity;
	//color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}