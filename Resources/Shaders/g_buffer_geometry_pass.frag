#version 400 

layout(location = 0) out vec3 g_position;
layout(location = 1) out vec3 g_normal;
layout(location = 2) out vec3 g_albedo;
layout(location = 3) out float g_frag_depth;
layout(location = 4) out float g_material_id;

const int NUM_CASCADES = 3;

in vec2 tex_coords;
in vec3 frag_pos;
in vec3 normal;
in float frag_depth;

//texture of rendered model; alway on TEXTRUE UNIT 0
uniform sampler2D model_texture;

uniform int material_id;


void main() {
    
	g_position      = frag_pos;
	g_normal        = normalize(normal);
    g_frag_depth    = frag_depth;
    g_material_id   = material_id;
    g_albedo        = vec3(1.f);//texture(model_texture, tex_coords).rgb;

}