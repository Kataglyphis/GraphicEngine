// worley noise implementation;
// inspired by Sebastian Lague: https://www.youtube.com/watch?v=4QOcCGI6xOU&t=104s

const int   NUM_FBM                 = 3;
const int   NUM_ADJACENT_CELLS      = 26;
const int   NUM_WORLEY_FREQUENCIES  = 3;

const int   NUM_CELL_POSITIONS      = 5;

const vec3 offsets[NUM_ADJACENT_CELLS + 1] =
{
    // centre
    vec3(0,0,0),
    // front face
    vec3(0,0,1),
    vec3(-1,1,1),
    vec3(-1,0,1),
    vec3(-1,-1,1),
    vec3(0,1,1),
    vec3(0,-1,1),
    vec3(1,1,1),
    vec3(1,0,1),
    vec3(1,-1,1),
    // back face
    vec3(0,0,-1),
    vec3(-1,1,-1),
    vec3(-1,0,-1),
    vec3(-1,-1,-1),
    vec3(0,1,-1),
    vec3(0,-1,-1),
    vec3(1,1,-1),
    vec3(1,0,-1),
    vec3(1,-1,-1),
    // ring around centre
    vec3(-1,1,0),
    vec3(-1,0,0),
    vec3(-1,-1,0),
    vec3(0,1,0),
    vec3(0,-1,0),
    vec3(1,1,0),
    vec3(1,0,0),
    vec3(1,-1,0)
};

float remap(float value, float start1, float stop1, float start2, float stop2) {

    return  start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));

}

float worley(   vec3        sample_id, 
                int         cell_index, 
                int         num_cells[NUM_CELL_POSITIONS],
                sampler3D   cell_positions[NUM_CELL_POSITIONS],
                float       resolution) {

    vec3 sample_pos = (sample_id / float(resolution)) * float(num_cells[cell_index]);

    vec3 cell_id = floor(sample_pos);

    float min_dist = 1.f;

    for (int i = 0; i < NUM_ADJACENT_CELLS + 1; i++) {

        vec3 adjacent_id = cell_id + offsets[i];

        bool out_of_scope = adjacent_id.x == -1 || adjacent_id.y == -1 || adjacent_id.z == -1 ||
            adjacent_id.x == num_cells[cell_index] || adjacent_id.y == num_cells[cell_index] ||
            adjacent_id.z == num_cells[cell_index];

        if (out_of_scope) {

            vec3 wrapped_id = mod(adjacent_id + vec3(num_cells[cell_index]), vec3(num_cells[cell_index]));
            vec3 wrapped_position = texture(cell_positions[cell_index], (wrapped_id) / float(num_cells[cell_index])).xyz;

            for (int m = 0; m < NUM_ADJACENT_CELLS; m++) {

                //after wrapping one has to check once again for lowest difference     
                vec3 difference = sample_pos - (wrapped_position + (offsets[m] * num_cells[cell_index]));
                min_dist = min(min_dist, dot(difference, difference));

            }


        }
        else {

            vec3 difference = sample_pos - texture(cell_positions[cell_index], adjacent_id / float(num_cells[cell_index])).xyz;
            min_dist = min(min_dist, dot(difference, difference));

        }

    }

    return sqrt(min_dist);

}

// really cool article: https://iquilezles.org/articles/fbm/
float fbm_worley(float worley_noise[NUM_WORLEY_FREQUENCIES], float H)
{

    float gain = exp2(-H);
    float amplitude = .6f;
    float result = 0.0f;

    for (int i = 0; i < NUM_WORLEY_FREQUENCIES; i++)
    {
        result += amplitude * worley_noise[i];
        amplitude *= gain;
    }

    return result;

}