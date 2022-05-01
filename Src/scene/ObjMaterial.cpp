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

	this->uniform_helper = UniformHelper();
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

	this->uniform_helper = UniformHelper();
}

void ObjMaterial::use_material(ObjMaterialLocations material_locations) {

	uniform_helper.setUniformVec3(ambient, material_locations.ambient_location);
	uniform_helper.setUniformVec3(diffuse, material_locations.diffuse_location);
	uniform_helper.setUniformVec3(specular, material_locations.specular_location);
	uniform_helper.setUniformVec3(transmittance, material_locations.transmittance_location);
	uniform_helper.setUniformVec3(emission, material_locations.emission_location);

	uniform_helper.setUniformFloat(shininess, material_locations.shininess_location);
	uniform_helper.setUniformFloat(ior, material_locations.ior_location);
	uniform_helper.setUniformFloat(dissolve, material_locations.dissolve_location);

	uniform_helper.setUniformInt(illum, material_locations.illum_location);
	uniform_helper.setUniformInt(textureID, material_locations.textureID_location);

}

ObjMaterial::~ObjMaterial() {



}