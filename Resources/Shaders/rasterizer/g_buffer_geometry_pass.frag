#version 460
#extension GL_ARB_shading_language_include : require

#include "/Globals.glsl"
#include "/host_device_shared.h"

layout(location = 0) out vec3   g_position;
layout(location = 1) out vec3   g_normal;
layout(location = 2) out vec3   g_albedo;
layout(location = 3) out float  g_frag_depth;
layout(location = 4) out float  g_material_id;

in vec2     tex_coords;
in vec3     frag_pos;
in vec3     normal;
in float    frag_depth;

layout(std430, binding = 5) buffer materialIndexPerPrimitive
{
    int materialIndex_SSBO[];
};

//all textures from the current model
uniform sampler2D model_textures[MAX_TEXTURE_COUNT];

void main() {
    
	g_position      = frag_pos;
	g_normal        = normalize(normal);
    g_frag_depth    = frag_depth;
    g_material_id   = materialIndex_SSBO[gl_PrimitiveID];
    g_albedo        = texture(model_textures[0], tex_coords).rgb;

}