#version 460

#extension GL_ARB_shading_language_include : require

#include "/Globals.glsl"
#include "/host_device_shared.h"

#include "/disney.glsl"
#include "/frostbite.glsl"
#include "/pbrBook.glsl"
#include "/phong.glsl"
#include "/unreal4.glsl"

in vec2 tex_coords;

#define NUM_MARCH_STEPS 32
#define NUM_MARCH_STEPS_TO_LIGHT 6

out vec4 color;

//define light uniforms here 
uniform DirectionalLight directional_light;
uniform OmniShadowMap omni_shadow_maps[MAX_POINT_LIGHTS];

//g_buffer data 
uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo;
uniform sampler2D g_frag_depth;
uniform sampler2D g_material_id;

//cloud world position and depth
uniform sampler2D cloud_position_depth;

//a set of random numbers for getting an good offset for raymarching
// with that we accomplish a round "random" shape for our cloud 
//in [0,1 ]
uniform sampler2D random_number;

//our directional shadow maps; 
uniform sampler2D directional_shadow_maps[NUM_CASCADES];
uniform mat4 directional_light_transform[NUM_CASCADES];

//sampler for our noise textures
uniform sampler3D noise_texture_1;
uniform sampler3D noise_texture_2;

//all other uniforms
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform float cascade_endpoints[NUM_CASCADES];
uniform int point_light_count;
uniform vec3 eye_position;
uniform Material materials[MAX_MATERIALS];
uniform Clouds cloud;
uniform mat4 VP;

uniform int num_active_cascades;
uniform int pcf_radius;

//all sample directions for our omni directional shader
vec3 sample_offset_directions[20] = vec3[] (
    vec3(1,1,1), vec3(1,-1,1), vec3(-1,-1,1), vec3(-1,1,1),
    vec3(1,1,-1), vec3(1,-1,-1), vec3(-1,-1,-1), vec3(-1,1,-1),
    vec3(1,1,0), vec3(1,-1,0), vec3(-1,-1,0), vec3(-1,1,0),
    vec3(1,0,1), vec3(-1,0,1), vec3(1,0,-1), vec3(-1,0,-1),
    vec3(0,1,1), vec3(0,-1,1), vec3(0,-1,-1), vec3(0,1,-1)
);

bool is_proj_coord_valid(vec3 proj_coord) {

    bool result = true;

    if (proj_coord.x < 0.0f || proj_coord.x > 1.f ||
        proj_coord.y < 0.0f || proj_coord.y > 1.f ||
        proj_coord.z < 0.0f || proj_coord.z > 1.f) result = false;

    return result;

}

float percentage_closer_shadow_filtering(int cascade_index, vec3 proj_coords) {
    
     //PCF
    float current_depth = proj_coords.z;
    float shadow = 0.0f;
    vec2 texel_size = 1.0f / textureSize(directional_shadow_maps[cascade_index], 0);
    float num_neighbors = 0.0f;

    for(int x = -pcf_radius; x <= pcf_radius; x++) {
        for(int y = -pcf_radius; y <= pcf_radius; y++) {
            
            vec3 proj_neighbor = vec3(proj_coords.xy + vec2(x,y) * texel_size, 1.0);

            if(is_proj_coord_valid(proj_neighbor)) {
            
                float closest_depth_neighbor = texture(directional_shadow_maps[cascade_index], proj_neighbor.xy).r;
                shadow += current_depth > closest_depth_neighbor + 0.001f  ? 1.0f : 0.0f;
                num_neighbors++;

            }

        }
    }

    shadow /= num_neighbors;

    if(proj_coords.z >= 1.0) {
        shadow = 0.0f;
    }

    return shadow;

}

vec3 get_proj_position_coords_from_cascade(int cascade_index) {
    
    vec3 proj_coords;
    vec4 pos_from_cascaded_point = directional_light_transform[cascade_index] * vec4(texture(g_position, tex_coords).rgb, 1.0f);
    proj_coords = pos_from_cascaded_point.rgb / pos_from_cascaded_point.a;
    proj_coords = (proj_coords * 0.5f) + 0.5f;

    return proj_coords;

}

bool valid_upper_neighbor(int cascade_index) {

    bool result = false;

    if ((cascade_index + 1 < num_active_cascades) &&  (is_proj_coord_valid(get_proj_position_coords_from_cascade(cascade_index + 1)))) {
    
        result = true;

    }

    return result;

}

bool valid_lower_neighbor(int cascade_index) {

    bool result = false;

    if (cascade_index - 1 >= 0 && ( is_proj_coord_valid(get_proj_position_coords_from_cascade(cascade_index - 1)))) {
    
        result = true;

    }

    return result;

}

//with PCF; cascaded approach
float calc_directional_shadow_factor(DirectionalLight d_light) {
    
    vec3 proj_coords = vec3(0.f);
    //float closest_depth = 0.0f;
    int cascade_index = 0;
    float frag_depth = texture(g_frag_depth, tex_coords).r;
    //proj_coords = get_proj_position_coords_from_cascade(0);
    

    for(int i = 0; i < NUM_CASCADES; i++) {

        if (frag_depth  <= cascade_endpoints[i]) {

            proj_coords = get_proj_position_coords_from_cascade(i);
            cascade_index = i;
            if (is_proj_coord_valid(proj_coords)) break;

        }

    }

    float shadow = percentage_closer_shadow_filtering(cascade_index, proj_coords);

//    if(valid_lower_neighbor(cascade_index)) {
//        
//        float lower_neighbor_shadow = percentage_closer_shadow_filtering(cascade_index - 1, get_proj_position_coords_from_cascade(cascade_index - 1));
//        //if we are in the first cascade it is simply 0
//        
//        float dist = (cascade_endpoints[cascade_index] - frag_depth) / (cascade_endpoints[cascade_index] - cascade_endpoints[cascade_index - 1]);
//
//        shadow = (dist) * lower_neighbor_shadow + (1.f - dist) * shadow;
//    
//    }
//
   if (valid_upper_neighbor(cascade_index)) {

        float upper_neighbor_shadow = percentage_closer_shadow_filtering(cascade_index + 1, get_proj_position_coords_from_cascade(cascade_index + 1));
        //if we are in the first cascade it is simply 0
        float cascade_start_point = 0.0f;
    
        if (cascade_index - 1 >= 0) {
        
            cascade_start_point = cascade_endpoints[cascade_index-1];
        
        } 

        float dist = (cascade_endpoints[cascade_index] - frag_depth) / (cascade_endpoints[cascade_index] - cascade_start_point);

        shadow = (1.f - dist) * upper_neighbor_shadow + ( dist) * shadow;

   }


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

    int mode = 2;
	switch (mode) {
	case 0: color += evaluteUnreal4PBR(ambient, N, L, V, roughness, light.color, light.ambient_intensity);
		break;
	case 1: color += evaluatePBRBooksPBR(ambient, N, L, V, roughness, light.color, light.ambient_intensity);
		break;
	case 2: color += evaluateDisneysPBR(ambient, N, L, V, roughness, light.color, light.ambient_intensity);
		break;
	case 3: color += evaluatePhong(ambient, N, L, V, light.color, light.ambient_intensity);
		break;		
	case 4: color += evaluateFrostbitePBR(ambient, N, L, V, roughness, light.color, light.ambient_intensity);
		break;
	}

    //(1.f - shadow_factor) * 
    return vec4(color,1.0f);

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
    float attentuation =  (p_light.exponent * dist * dist) 
                                     +  (p_light.linear * dist);

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

float sample_density(vec3 position) {
    
    vec3 uvw = (mod(position + cloud.offset , 128)) / 128.f; 
    vec4 noise_128= texture(noise_texture_1, uvw);
   
    float base_density = (0.75f + (cloud.pillowness * 0.25f)) * max(0.0f, noise_128.r - cloud.threshold) * cloud.scale
                            + ((1.f - cloud.pillowness) * 0.125f) * max(0.0f, noise_128.g - cloud.threshold) * cloud.scale
                            + ((1.f - cloud.pillowness) * 0.125f) * max(0.0f, noise_128.b - cloud.threshold) * cloud.scale;
    
    base_density = base_density * (1.f - cloud.cirrus_effect) + max(0.0f, noise_128.a - cloud.threshold) * cloud.scale * (cloud.cirrus_effect);

    vec3 uvw_32 = (mod(position+ cloud.offset, 32)) /32.f; // + cloud.offset
    vec4 noise_32= texture(noise_texture_2, uvw_32);

    float fine_density = 0.6f * max(0.0f, noise_32.r - cloud.threshold) * cloud.scale
                            + 0.15f * max(0.0f, noise_32.g - cloud.threshold) * cloud.scale
                            + 0.15f * max(0.0f, noise_32.b - cloud.threshold) * cloud.scale;

    return  base_density;// + fine_density * 0.3f;

}

float phase_HG (float cosTheta, float g) {

    float denom = 1 + g * g + 2 * g * cosTheta;
    return (1.f/(4.f * PI)) * ((1 - g * g) / (denom * sqrt(denom)));

}

float light_march(vec3 sample_pos) {
    
    float total_density = 0.0f;
    float light_absorption = 1.0f;
    float henyey_greenstein_phase_function = 2.f;

    vec3 direction_to_light = normalize(directional_light.direction);
    vec2 oT, oU;
    vec3 oN;
    int oF;
    bool intersection = box_intersect_with_ray(eye_position, direction_to_light, inverse(cloud.model_to_world), 
                                                    cloud.model_to_world, cloud.rad, oT, oN, oU, oF); 

    for(int i  = 0; i < NUM_MARCH_STEPS_TO_LIGHT; i++) {

        float step_size = (float(i)/float(NUM_MARCH_STEPS_TO_LIGHT)) * (oT.y - oT.x);
        sample_pos += direction_to_light * (step_size);
        total_density = sample_density(sample_pos); 

    }

    total_density /= NUM_MARCH_STEPS_TO_LIGHT;

    float transmittance = exp(-total_density * light_absorption);

    return transmittance;

}

void calc_clouds() {

    float frag_depth = texture(g_frag_depth, tex_coords).r;
    vec3 frag_pos = texture(g_position, tex_coords).rgb;
    bool hit_skybox = false;

    if (frag_depth > 0.0f) {

        frag_pos = texture(g_position, tex_coords).rgb;

    } else {

        hit_skybox = true;
        // if the fragment depth is zero then it is part of our sky box 
        // get world position by using VP^-1 and calculate further with it 
        vec4 cloud_pos_and_depth = texture(cloud_position_depth, tex_coords);
        frag_pos = cloud_pos_and_depth.xyz;
        //also adjust the frag_depth 
        frag_depth = cloud_pos_and_depth.w;

    }

    vec3 ray_direction = normalize(frag_pos - eye_position); //
    vec2 oT, oU;
    vec3 oN;
    int oF;
    bool intersection = box_intersect_with_ray(eye_position, ray_direction, inverse(cloud.model_to_world), 
                                                    cloud.model_to_world, cloud.rad, oT, oN, oU, oF); 

    float light_energy = 0.0f;
    float transmittance = 1.f;
    //color = vec4(oT.y - oT.x, 0.0f,0.0f,1.0f);

    float far_plane = 0.0;
    
    for (int i = 0; i < NUM_CASCADES; i++) {
    
        far_plane = max(far_plane, cascade_endpoints[i]);
    
    }

    if (intersection && ((frag_depth > oT.x && hit_skybox == false) || 
       (frag_depth > oT.x - far_plane && hit_skybox == true))) {
        
        //the cloud model is in [-1,1]^3
        //therefore going into model space gives us something like a scale 

        float rnd_numb = texture(random_number, tex_coords).r;

        for(int i  = 0; i < NUM_MARCH_STEPS; i++) {
            
            float step_size = (float(i)/float(NUM_MARCH_STEPS)) * (oT.y - oT.x);
            vec3 sample_pos = eye_position + ray_direction * (oT.x + step_size);
            float density_of_sample = sample_density(sample_pos); 
            float light_transmittance = light_march(sample_pos);

            light_energy += density_of_sample * step_size * transmittance * light_transmittance *
                                                                                                phase_HG(dot(normalize(ray_direction), normalize(-directional_light.direction)), 0.5f);

            transmittance *= exp(-density_of_sample * step_size);

            if( transmittance < 0.01f) {
                break;
            }

            if(cloud.powder_effect){

                float powderness = 1.0f - exp(-(density_of_sample * step_size)/2.f);
                transmittance += powderness;

            }

        }
        
        //vec3 point_in_cloud_space = (inverse(cloud.model_to_world) * vec4(frag_pos,1.0f)).xyz;
        //float euclidian_dist = length(point_in_cloud_space * 0.5f);
        vec3 cloud_color = light_energy * directional_light.base.color * directional_light.base.ambient_intensity;
        color = color * (transmittance) + vec4(cloud_color, 1.0f);
        //color = vec4(abs(oT.x), abs(oT.y),0.0f,1.0f);
        //color = vec4(frag_pos,1.0f);
        //color = vec4(point_in_cloud_space, 1.0f);
    }

}

void debug_cascaded_shadow_maps() {

     vec4 pos_from_cascaded_point_1 = directional_light_transform[0] * vec4(texture(g_position, tex_coords).rgb, 1.0f);
    vec3 pos_from_cascaded_point_1_proj = pos_from_cascaded_point_1.rgb / pos_from_cascaded_point_1.a;
    pos_from_cascaded_point_1_proj = (pos_from_cascaded_point_1_proj * 0.5f) + 0.5f;
    float closest_depth_first_cascade = 0;
    if(is_proj_coord_valid(pos_from_cascaded_point_1_proj)) closest_depth_first_cascade = 
                                                texture(directional_shadow_maps[0], pos_from_cascaded_point_1_proj.xy).r;

    vec4 pos_from_cascaded_point_2 = directional_light_transform[1] * vec4(texture(g_position, tex_coords).rgb, 1.0f);
    vec3 pos_from_cascaded_point_2_proj = pos_from_cascaded_point_2.rgb / pos_from_cascaded_point_2.a;
    pos_from_cascaded_point_2_proj = (pos_from_cascaded_point_2_proj * 0.5f) + 0.5f;
    float closest_depth_second_cascade = 0;
    if(is_proj_coord_valid(pos_from_cascaded_point_2_proj)) closest_depth_second_cascade = 
                                                texture(directional_shadow_maps[1], pos_from_cascaded_point_2_proj.xy).r;

    vec4 pos_from_cascaded_point_3 = directional_light_transform[2] * vec4(texture(g_position, tex_coords).rgb, 1.0f);
    vec3 pos_from_cascaded_point_3_proj = pos_from_cascaded_point_3.rgb / pos_from_cascaded_point_3.a;
    pos_from_cascaded_point_3_proj = (pos_from_cascaded_point_3_proj * 0.5f) + 0.5f;
    float closest_depth_third_cascade = 0;
    if(is_proj_coord_valid(pos_from_cascaded_point_3_proj)) closest_depth_third_cascade = 
                                                texture(directional_shadow_maps[2], pos_from_cascaded_point_3_proj.xy).r;
    
    float frag_depth = texture(g_frag_depth, tex_coords).r;
    //color = vec4(1.f - closest_depth_first_cascade, 1.f - closest_depth_second_cascade, 1.f - closest_depth_third_cascade, 1.0f); //
    //color = vec4((cascade_endpoints[0] - frag_depth),0.f,0.f,1.0f);
    color = vec4(closest_depth_first_cascade, 0.0f,0.0f, 1.0f);
    //color = vec4(0.0f, closest_depth_second_cascade,0.0f, 1.0f);
    //color = vec4(0.0f, 0.0f, closest_depth_third_cascade, 1.0f);
    //color = vec4(closest_depth_first_cascade, closest_depth_second_cascade, closest_depth_third_cascade, 1.0f);

}


void main () {
    
    vec4 final_color = calc_directional_light();
    // final_color += calc_point_lights();

    //do not shade when skybox is visible!
//    float frag_depth = texture(g_frag_depth, tex_coords).r;
//
//    if(frag_depth > 0.0f) {
//
//        color = final_color;
//
//     } else {
//
//        color = texture(g_albedo, tex_coords);
//
//    }
//    
//    color = vec4(gamma_correction(color.xyz),1.0);
//
//    calc_clouds();
    color = final_color;
    //debug_cascaded_shadow_maps();

}

