#pragma once
#include "host_device_shared.h"
#include "GlobalValues.h"

// define all texture unit offsets to all textures
#define MODEL_TEXTURES_SLOT				0
#define SKYBOX_TEXTURES_SLOT			MODEL_TEXTURES_SLOT + MAX_TEXTURE_COUNT
#define GBUFFER_TEXTURES_SLOT			SKYBOX_TEXTURES_SLOT + 1 // we have 1 skybox 3D texture
#define D_LIGHT_SHADOW_TEXTURES_SLOT	GBUFFER_TEXTURES_SLOT + G_BUFFER_SIZE
// for the CSM now using 1 sampler array layer --> only one texture unit needed
#define P_LIGHT_SHADOW_TEXTURES_SLOT	D_LIGHT_SHADOW_TEXTURES_SLOT + 1
#define NOISE_128D_TEXTURES_SLOT		P_LIGHT_SHADOW_TEXTURES_SLOT + MAX_POINT_LIGHTS
#define NOISE_32D_TEXTURES_SLOT			NOISE_128D_TEXTURES_SLOT + 1
#define NOISE_CELL_POSITIONS_SLOT		NOISE_32D_TEXTURES_SLOT + 1
#define RANDOM_NUMBERS_SLOT				NOISE_CELL_POSITIONS_SLOT + NUM_CELLS			