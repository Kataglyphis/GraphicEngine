#pragma once
#include <glew.h>

struct ObjMaterialLocations {

	GLuint ambient_location;
	GLuint diffuse_location;
	GLuint specular_location;
	GLuint transmittance_location;
	GLuint emission_location;
	GLuint shininess_location;
	GLuint ior_location;
	GLuint dissolve_location;

	GLuint illum_location = 0;
	GLuint textureID_location = -1;

};