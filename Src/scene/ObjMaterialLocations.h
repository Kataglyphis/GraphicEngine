#pragma once
#include <glew.h>

struct ObjMaterialLocations {

	GLint ambient_location;
	GLint diffuse_location;
	GLint specular_location;
	GLint transmittance_location;
	GLint emission_location;
	GLint shininess_location;
	GLint ior_location;
	GLint dissolve_location;

	GLint illum_location = 0;
	GLint textureID_location = -1;

};