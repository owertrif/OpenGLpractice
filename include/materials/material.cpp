#include "material.h"

Material::Material(unsigned int textureID, int textureUnit, float specr, float specg, float specb, float shinines){
    this->textureID = textureID;
    this->textureUnit = textureUnit;
    this->specular = glm::vec3(specr,specg,specb);
    this->shininess = shininess;
}

Material::Material(unsigned int textureID,int textureUnit, glm::vec3 specular, float shininess){
    this->textureID = textureID;
    this->textureUnit = textureUnit;
    this->specular = specular;
    this->shininess = shininess;
}