struct Clouds {

    vec3 rad;
    vec3 offset;
    mat4 model_to_world;

    float threshold;
    float scale;
    float pillowness;
    float cirrus_effect;

    bool powder_effect;

};

struct Light {

    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;

};

struct DirectionalLight {

    Light base;
    vec3 direction;
    float shadow_intensity;

};

struct PointLight {

    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;

};

struct OmniShadowMap {

    samplerCube shadow_map;
    float far_plane;

};

struct Material {

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 transmittance;
	vec3 emission;
	float shininess;
	float ior;		// index of refraction
	float dissolve;	// 1 == opaque; 0 == fully transparent

	int illum;
	int textureID;

};