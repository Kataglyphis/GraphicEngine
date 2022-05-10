#version 460

#extension GL_ARB_shading_language_include : require

#include "/Globals.glsl"
#include "/host_device_shared.h"

#include "/Matlib.glsl"

#include "/microfacet.glsl"
#include "/disney.glsl"
#include "/frostbite.glsl"
#include "/pbrBook.glsl"
#include "/phong.glsl"
#include "/unreal4.glsl"

#include "/clouds.glsl"

in vec2 tex_coords;

out vec4 color;

//g_buffer data 
uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo;
uniform sampler2D g_material_id;

//all regarding the sun and its shadows
uniform DirectionalLight    directional_light;
uniform sampler2DArray      directional_shadow_maps;
uniform float               cascade_endpoints[NUM_CASCADES];
uniform int                 num_active_cascades;
uniform int                 pcf_radius;
layout (std140, binding = UNIFORM_LIGHT_MATRICES_BINDING) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[NUM_CASCADES];
};


//sampler for our noise textures
uniform Clouds      cloud;
uniform sampler3D   noise_texture_1;
uniform sampler3D   noise_texture_2;
uniform int         cloudsMaterialID;
// a set of random numbers for getting an good offset for raymarching
// with that we accomplish a round "random" shape for our cloud 
// in [0,1]
uniform sampler2D   random_number;

//all other uniforms
uniform PointLight      point_lights[MAX_POINT_LIGHTS];
uniform int             point_light_count;
uniform OmniShadowMap   omni_shadow_maps[MAX_POINT_LIGHTS];

uniform Material    materials[MAX_MATERIALS];
uniform int         skyBoxMaterialID;

uniform vec3 eye_position;
uniform mat4 view;
uniform mat4 projection;


//all sample directions for our omni directional shader
vec3 sample_offset_directions[20] = vec3[] (
    vec3(1,1,1), vec3(1,-1,1), vec3(-1,-1,1), vec3(-1,1,1),
    vec3(1,1,-1), vec3(1,-1,-1), vec3(-1,-1,-1), vec3(-1,1,-1),
    vec3(1,1,0), vec3(1,-1,0), vec3(-1,-1,0), vec3(-1,1,0),
    vec3(1,0,1), vec3(-1,0,1), vec3(1,0,-1), vec3(-1,0,-1),
    vec3(0,1,1), vec3(0,-1,1), vec3(0,-1,-1), vec3(0,1,-1)
);

float percentage_closer_shadow_filtering(int cascade_index, vec3 proj_coords) {
    
     //PCF
    float current_depth = proj_coords.z;
    float shadow = 0.0f;
    vec2 texel_size = 1.0f / textureSize(directional_shadow_maps, 0).xy;
    float num_neighbors = 0.0f;

    for(int x = -pcf_radius; x <= pcf_radius; x++) {
        for(int y = -pcf_radius; y <= pcf_radius; y++) {
            
            vec3 proj_neighbor = vec3(proj_coords.xy + vec2(x,y) * texel_size, 1.0);
            float closest_depth_neighbor = texture(directional_shadow_maps, vec3(proj_coords.xy + 
                                                    vec2(x, y) * texel_size, cascade_index)).r;
            shadow += current_depth > closest_depth_neighbor + 0.001f  ? 1.0f : 0.0f;
            num_neighbors++;

        }
    }

    shadow /= num_neighbors;

    if(proj_coords.z >= 1.0) {
        shadow = 0.0f;
    }

    return shadow;

}

//with PCF; cascaded approach
float calc_directional_shadow_factor(DirectionalLight d_light) {
    
    vec4 fragPosWorldSpace = vec4(texture(g_position, tex_coords).xyz, 1.0f);
    vec4 fragPosViewSpace = view * fragPosWorldSpace;
    float frag_depth = abs(fragPosViewSpace.z);
    int cascade_index = -1;

    for(int i = 0; i < NUM_CASCADES; i++) {
        if (frag_depth  < cascade_endpoints[i]) {
            cascade_index = i;
            break;

        }

    }

    if(cascade_index == -1) cascade_index = NUM_CASCADES;

    vec4 fragPosLightSpace = lightSpaceMatrices[cascade_index] * fragPosWorldSpace;

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(texture(g_normal, tex_coords).rgb);
    float bias = max(0.05 * (1.0 - dot(normal, d_light.direction)), 0.005);
    const float biasModifier = 0.5f;
    if (cascade_index == NUM_CASCADES)
    {
        float farPlane = cascade_endpoints[NUM_CASCADES - 1];
        bias *= 1 / (farPlane * biasModifier);
    }
    else
    {
        bias *= 1 / (cascade_endpoints[cascade_index] * biasModifier);
    }

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(directional_shadow_maps, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(directional_shadow_maps, vec3(projCoords.xy + vec2(x, y) * texelSize, cascade_index)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }

    shadow /= 9.0;
        
    return shadow;
}

vec4 calc_light_by_direction(Light light, vec3 direction, float shadow_factor) {

    int material_id = int(texture(g_material_id, tex_coords).r);
    vec3 ambient    = texture(g_albedo, tex_coords).rgb;
    vec3 frag_pos   = texture(g_position, tex_coords).rgb;
    vec3 N          = normalize(texture(g_normal, tex_coords).rgb);

    vec3 V          = normalize(eye_position - frag_pos);
    vec3 L          = normalize(-direction);
    vec3 H          = normalize(V+L);
    
    float roughness = 0.5f;

    vec3 color = vec3(0.f);

    int mode = 4;
	switch (mode) {
	case 0: color += evaluteUnreal4PBR(ambient, N, L, V, roughness, light.color, light.radiance);
		break;
	case 1: color += evaluatePBRBooksPBR(ambient, N, L, V, roughness, light.color, light.radiance);
		break;
	case 2: color += evaluateDisneysPBR(ambient, N, L, V, roughness, light.color, light.radiance);
		break;
	case 3: color += evaluatePhong(ambient, N, L, V, light.color, light.radiance);
		break;		
	case 4: color += evaluateFrostbitePBR(ambient, N, L, V, roughness, light.color, light.radiance);
		break;
	}

    return (1.f - shadow_factor) * vec4(color,1.0f);

}

vec4 calc_directional_light() {
    
    float shadow_factor = calc_directional_shadow_factor(directional_light);
    return calc_light_by_direction(directional_light.base, directional_light.direction, shadow_factor);

}

float calc_omni_shadow_factor(vec3 frag_pos, PointLight p_light, int shadow_index) {
    
    vec3 frag_to_light = frag_pos - p_light.position;
    float current_depth = length(frag_to_light);

    float shadow = 0.0f;
    float bias = 0.05f;
    int num_samples = 20;

    float view_distance = length(eye_position - frag_pos);
    float disk_radius = (1.0 + (view_distance/omni_shadow_maps[shadow_index].far_plane)) / 25.0;

    //PCF
    for(int i = 0; i < num_samples; i++) {
        float closest_depth = texture(omni_shadow_maps[shadow_index].shadow_map, frag_to_light + sample_offset_directions[i] * disk_radius).r;
                closest_depth *= omni_shadow_maps[shadow_index].far_plane;
                if(current_depth - bias > closest_depth) {
                    shadow += 1.0f;
                }
    }

    shadow /= float(num_samples); 

    return shadow;

}

vec4 calc_point_light(vec3 frag_pos, PointLight p_light, int shadow_index) {
        
    vec3 direction = p_light.position - frag_pos;
    float dist = length(direction);
    direction = normalize(direction);

    float shadow_factor = calc_omni_shadow_factor(frag_pos, p_light, shadow_index);

    vec4 color = vec4(p_light.base.color, 1.0f);//calc_light_by_direction(p_light.base, direction, shadow_factor); // 
    //float attentuation =1.0f * dist ;//pow(distance,2) ;
    float attentuation =  (p_light.exponent * dist * dist) +  (p_light.linear * dist);

    return (color / attentuation);

}

vec4 calc_point_lights() {

    vec3 frag_pos = texture(g_position, tex_coords).rgb;
    vec4 total_color = vec4(0.0f,0.0f,0.0f,0.0f);

    for(int i = 0; i < point_light_count; i++) {

        total_color += calc_point_light(frag_pos, point_lights[i], i);

    }

    return total_color;
}

bool belongs_to_skybox(int material_id) {
    
    return material_id == skyBoxMaterialID;

}

bool belongs_to_clouds(int material_id) {

    return (material_id == cloudsMaterialID);

}

void main () {
    
    vec4 albedo     = texture(g_albedo, tex_coords);
    vec4 frag_pos   = texture(g_position, tex_coords);
    int material_id = int(texture(g_material_id, tex_coords).r);
    float rnd_numb  = texture(random_number, tex_coords).r;

    vec4 final_color = calc_directional_light();
    // final_color += calc_point_lights();

    if(belongs_to_skybox(material_id) || belongs_to_clouds(material_id)) {
        // here we only hit the skybox/ or clouds; avoid shading!
        color = albedo;
    } else {
        color = final_color;
    }

    if(belongs_to_clouds(material_id)) {
        
        float far_plane = cascade_endpoints[NUM_CASCADES - 1];
        //color = vec4(1.f, 0.f,0.f, 1.0f);
        calc_clouds(projection, view, 
                    frag_pos, eye_position, 
                    cloud,
                    rnd_numb,
                    noise_texture_1, 
                    noise_texture_2, 
                    directional_light,
                    far_plane,
                    color);
    }

    color.xyz = Reinhards_tonemapping(color.xyz);
    color = vec4(gamma_correction(color.xyz),1.0);

}

