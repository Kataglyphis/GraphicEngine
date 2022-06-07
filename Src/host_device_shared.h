#ifndef GLOBALS_HOST_DEVICE
#define GLOBALS_HOST_DEVICE

// lights
const int NUM_CASCADES = 3;
const int MAX_POINT_LIGHTS = 1;

// materials
const int MAX_MATERIALS = 35;
const int SKYBOX_MATERIAL_ID = MAX_MATERIALS;
const int CLOUDS_MATERIAL_ID = MAX_MATERIALS + 1;

// texture
const int MAX_TEXTURE_COUNT = 30;

// math
const float PI = 3.14159265359f;

// noise
const int NUM_CELL_POSITIONS = 5;

#endif  // !GLOBALS_HOST_DEVICE
