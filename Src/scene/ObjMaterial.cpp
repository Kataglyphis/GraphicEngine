#include "ObjMaterial.h"

ObjMaterial::ObjMaterial() {

	this->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	this->diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	this->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	this->transmittance = glm::vec3(0.0f, 0.0f, 0.0f);
	this->emission = glm::vec3(0.0f, 0.0f, 0.10);
	this->shininess = 0.f;
	this->ior = 1.0f;
	this->dissolve = 1.f;
	this->illum = 0;
	this->textureID = -1;
}

ObjMaterial::ObjMaterial(	glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 transmittance,
							glm::vec3 emission, float shininess, float ior, float dissolve, int illum, int textureID)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->transmittance = transmittance;
	this->emission = emission;
	this->shininess = shininess;
	this->ior = ior;
	this->dissolve = dissolve;
	this->illum = illum;
	this->textureID = textureID;
}

void ObjMaterial::use_material(ObjMaterialLocations material_locations) {

	glUniform3f(material_locations.ambient_location, ambient.x, ambient.y, ambient.z);
	glUniform3f(material_locations.diffuse_location, diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(material_locations.specular_location, specular.x, specular.y, specular.z);
	glUniform3f(material_locations.transmittance_location, transmittance.x, transmittance.y, transmittance.z);
	glUniform3f(material_locations.emission_location, emission.x, emission.y, emission.z);

	glUniform1f(material_locations.shininess_location, shininess);
	glUniform1f(material_locations.ior_location, ior);
	glUniform1f(material_locations.dissolve_location, dissolve);

	glUniform1f(material_locations.illum_location, illum);
	glUniform1f(material_locations.textureID_location, textureID);

}

ObjMaterial::~ObjMaterial() {



}