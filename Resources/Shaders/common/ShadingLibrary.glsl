#include "Matlib.glsl"

#ifndef SHADING_LIB
#define SHADING_LIB

vec3 Reinhards_tonemapping(vec3 color) {

    return color / (color + vec3(1.f));

}

vec3 gamma_correction(vec3 color) {

    return pow(color / (color + vec3(1.0f)), vec3(1.0f / 2.2f));

}

vec3 LambertDiffuse(vec3 ambient) {

    return ambient / PI;

}

vec3 fresnel_schlick(float cosTheta, vec3 ambient_color, float metallic) {

    vec3 F0 = mix(vec3(0.04), ambient_color, metallic);
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
    return F;

}

// for the case u want to visualize depth
// otherwise u only look on white texture :)
float LinearizeDepth(float depth, float near_plane, float far_plane)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}
#endif