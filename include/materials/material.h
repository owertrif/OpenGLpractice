#ifndef MATERIAL_H
#define MATERIAL_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

class Material{
public:
    Material(glm::vec3 ambient(0.0f,0.0f,0.0f), glm::vec3 diffuse(0.0f,0.0f,0.0f), glm::vec3::specular(0.0f,0.0f,0.0f), float shininess) : this->ambient(ambient),
                                                                                                                                           this->diffuse(diffuse),
                                                                                                                                           this->specular(specular),
                                                                                                                                           this->shininess(shininess);
                                                                                                                                           
    Material(float ambr, float ambg, float ambb,float diffr, float diffg, float diffb,float specr, float specg, float specb, float shinines);


    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
}

Material::Material(float ambr, float ambg, float ambb,float diffr, float diffg, float diffb,float specr, float specg, float specb, float shinines){
    this->ambient = glm::vec3(ambr,ambg,ambb);
    this->diffuse = glm::vec3(diffr,diffg,diffb);
    this->specular = glm::vec3(specr,specg,specb);
    this->shininess = shininess;
}

#endif