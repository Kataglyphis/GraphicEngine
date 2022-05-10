#include "/host_device_shared.h"
#include "/Matlib.glsl"
#include "/Globals.glsl"

#ifndef CLOUDS
#define CLOUDS

#define NUM_MARCH_STEPS 32
#define NUM_MARCH_STEPS_TO_LIGHT 6

struct Clouds {

    vec3    rad;
    vec3    offset;
    mat4    model_to_world;

    float   threshold;
    float   scale;
    float   pillowness;
    float   cirrus_effect;

    bool    powder_effect;

};

float sample_density(   vec3 position, Clouds cloud, 
                        sampler3D noise_texture_1, 
                        sampler3D noise_texture_2) {

    vec3 uvw            =   (mod(position + cloud.offset, 128)) / 128.f;
    vec4 noise_128      =   texture(noise_texture_1, uvw);

    float base_density  =   (0.75f + (cloud.pillowness * 0.25f)) * max(0.0f, noise_128.r - cloud.threshold) * cloud.scale
                            + ((1.f - cloud.pillowness) * 0.125f) * max(0.0f, noise_128.g - cloud.threshold) * cloud.scale
                            + ((1.f - cloud.pillowness) * 0.125f) * max(0.0f, noise_128.b - cloud.threshold) * cloud.scale;

    base_density        =   base_density * (1.f - cloud.cirrus_effect) + 
                            max(0.0f, noise_128.a - cloud.threshold) * cloud.scale * (cloud.cirrus_effect);

    vec3 uvw_32         =   (mod(position + cloud.offset, 32)) / 32.f; // + cloud.offset
    vec4 noise_32       =   texture(noise_texture_2, uvw_32);

    float fine_density  =    0.6f * max(0.0f, noise_32.r - cloud.threshold) * cloud.scale
                            + 0.15f * max(0.0f, noise_32.g - cloud.threshold) * cloud.scale
                            + 0.15f * max(0.0f, noise_32.b - cloud.threshold) * cloud.scale;

    return  base_density;// + fine_density * 0.3f;

}

float phase_HG(float cosTheta, float g) {

    float denom = 1 + g * g + 2 * g * cosTheta;
    return (1.f / (4.f * PI)) * ((1 - g * g) / (denom * sqrt(denom)));

}

float light_march(  vec3 sample_pos,
                    vec3 eye_position,
                    DirectionalLight directional_light,
                    Clouds cloud, 
                    sampler3D noise_texture_1,
                    sampler3D noise_texture_2) {

    float total_density = 0.0f;
    float light_absorption = 1.0f;
    float henyey_greenstein_phase_function = 2.f;

    vec3 direction_to_light = normalize(-directional_light.direction);
    vec2 oT, oU;
    vec3 oN;
    int oF;
    bool intersection = box_intersect_with_ray( eye_position, 
                                                direction_to_light, 
                                                inverse(cloud.model_to_world),
                                                cloud.model_to_world, 
                                                cloud.rad,
                                                oT, oN, oU, oF);

    for (int i = 0; i < NUM_MARCH_STEPS_TO_LIGHT; i++) {

        float step_size = (float(i) / float(NUM_MARCH_STEPS_TO_LIGHT)) * (oT.y - oT.x);
        sample_pos += direction_to_light * (step_size);

        total_density = sample_density( sample_pos,
                                        cloud,
                                        noise_texture_1,
                                        noise_texture_2);

    }

    total_density /= NUM_MARCH_STEPS_TO_LIGHT;

    float transmittance = exp(-total_density * light_absorption);

    return transmittance;

}

void calc_clouds(   mat4 projection, mat4 view, 
                    vec4 frag_pos, vec3 eye_position, 
                    Clouds cloud, float rnd_numb, 
                    sampler3D noise_texture_1,
                    sampler3D noise_texture_2, 
                    DirectionalLight directional_light,
                    float far_plane,
                    inout vec4 color) {

    float frag_depth = (projection * view * frag_pos).z;

    vec3 ray_direction = normalize(frag_pos.xyz - eye_position);
    vec2 oT, oU;
    vec3 oN;
    int oF;
    bool intersection = box_intersect_with_ray( eye_position, ray_direction, 
                                                inverse(cloud.model_to_world),
                                                cloud.model_to_world, cloud.rad, 
                                                oT, oN, oU, oF);

    float light_energy = 0.0f;
    float transmittance = 1.f;

    if (intersection && ((frag_depth > oT.x))) {

        for (int i = 0; i < NUM_MARCH_STEPS; i++) {

            float step_size = (float(i) / float(NUM_MARCH_STEPS)) * (oT.y - oT.x);
            vec3 sample_pos = eye_position + ray_direction * (oT.x + step_size);

            float density_of_sample = sample_density(   sample_pos, cloud, 
                                                        noise_texture_1, noise_texture_2);

            float light_transmittance = light_march(sample_pos, 
                                                    eye_position,
                                                    directional_light, 
                                                    cloud, 
                                                    noise_texture_1,
                                                    noise_texture_2);

            light_energy += density_of_sample   * 
                            step_size           * 
                            transmittance       * 
                            light_transmittance *
                            phase_HG(dot(normalize(ray_direction), normalize(directional_light.direction)), 0.5f);

            transmittance *= exp(-density_of_sample * step_size);

            if (transmittance < 0.01f) {
                break;
            }

            if (cloud.powder_effect) {

                float powderness = 1.0f - exp(-(density_of_sample * step_size) / 2.f);
                transmittance += powderness;

            }

        }

        //vec3 point_in_cloud_space = (inverse(cloud.model_to_world) * frag_pos).xyz;
        //float euclidian_dist = length(point_in_cloud_space * 0.5f);
        vec3 cloud_color = light_energy * directional_light.base.color * directional_light.base.radiance;
        color = color * (transmittance) + vec4(cloud_color, 1.0f);//
        //color = vec4(abs(oT.x), abs(oT.y),0.0f,1.0f);
        //color = frag_pos;
        //color = vec4(point_in_cloud_space, 1.0f);
    }

}

#endif