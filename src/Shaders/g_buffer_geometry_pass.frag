#version 330 

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

// multio tex variables 
uniform bool isTerrainTexture = false;
uniform float max_biom_heights[8];
uniform float max_height;
// Texture for the terrain
uniform sampler2D biom_texture[8];

void main() {
    
	g_position = frag_pos;
	g_normal = normalize(normal);
    g_frag_depth = frag_depth;
    g_material_id = material_id;
//    g_albedo = texture(model_texture, tex_coords).rgb; // Merge conflict


    //  decide Normal texture or terrain texture
    if (isTerrainTexture) {
       //multio tex ---------------------------------------

       // negativ height? -> to 0?


        // normalize height // Note + 25.0 is hard coded and try to negate Terrain transformation
        float height_in_percent = ((g_position.y /* + 25.0f*/ )) / (max_height);
//        if (height_in_percent < 0) height_in_percent = 0.0f;
        // biom index
        int biom_index = max_biom_heights.length()-1;
        // calculate terrain

        // distance between next
        for(int i = 1; i< max_biom_heights.length(); i++) {
            if(height_in_percent <= max_biom_heights[i]) {
                biom_index = i; // it should be -1
//                biom_index = i - 1; // it should be correct

                break;
            }
            // if the max height aren't correct (sometimes height can heigher than max)
            biom_index = i;
        }


        float alpha = texture(biom_texture[biom_index], tex_coords).rgba.a;
        if (alpha <= 0.1f) {
            discard;
        }


        //transition ------------------------------------------------------------

        int calc_index = biom_index - 1;
        float distance_between_lower_and_verHeight_biom_in_perc = 0.0f;
        if (biom_index < max_biom_heights.length() - 1) {
            // all biom without last one
           float heigher_biom_height = max_biom_heights[calc_index + 1];
           float thisBiomHeight =  heigher_biom_height - max_biom_heights[calc_index];
           distance_between_lower_and_verHeight_biom_in_perc = (height_in_percent - max_biom_heights[calc_index]) / thisBiomHeight;
        }

        // at this percentage, make a transition between the other textures
        float transition_gap = 0.85f; 

        // check if the texture are on the edge of the next terrain texture
        if (distance_between_lower_and_verHeight_biom_in_perc >= transition_gap) {
            // Transition in the top 15% -> trans_a = relative delta in this 10% area = 1.0f means use 0 % thisTex and 100% the next Texture, 0.0f means use 100% of thisTex and 0% of the heiger Biom Tex
            float trans_a = (distance_between_lower_and_verHeight_biom_in_perc - transition_gap)/ (1-transition_gap);  
            
               // get psyeudo random float number
               // dot product (skalarprodukt) with a big random number mutltyply it with a big number, and use Sinus function
               float psyRandomNumber = fract(sin(dot(tex_coords, vec2(12.9898, 78.233))) * 43758.5453);


               
               // trans_a = probalility of next texture (Bim texture + 1)
               if (psyRandomNumber <= trans_a) {
                    // is in the Probability of trans_a
                    g_albedo = texture(biom_texture[biom_index + 1], tex_coords).rgb;
               } else {
                    // is out of probability of trans_a
                    g_albedo = texture(biom_texture[biom_index], tex_coords).rgb;
               }

               // render the transition eare black.
//               g_albedo = vec3(0.0f,0.0f,0.0f);

        } else {
            g_albedo = texture(biom_texture[biom_index], tex_coords).rgb;
        }   //-----End multio texturing ---------------------------------------
    } else {
        // ----- normal texturing ------
	    float alpha = texture(model_texture, tex_coords).rgba.a;
        if (alpha <= 0.1f) {
            discard;
        }
	    g_albedo = texture(model_texture, tex_coords).rgb;
    }


    g_frag_depth = frag_depth;
    g_material_id = material_id;

}