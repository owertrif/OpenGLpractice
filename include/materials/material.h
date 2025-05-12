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
    unsigned int textureID;
    int textureUnit;
    glm::vec3 specular;
    float shininess;

    Material(unsigned int textureID = 0, int textureUnit = 0, glm::vec3 specular = glm::vec3(0.0f,0.0f,0.0f), float shininess = 0.0f);                                                                                                                                   
    Material(unsigned int textureID, int textureUnit,float specr, float specg, float specb, float shinines);
};

#endif