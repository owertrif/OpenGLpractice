#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "math.h"
#include "shader/shader.h"
#include "camera/camera.h"
#include "materials/material.h"
#include "std_img/stb_image_implementation.h"

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window,int width, int heigh);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
unsigned int loadTexture(const char* path);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f,0.0f,3.0f));
float lastX = SCR_WIDTH/2.0f, lastY = SCR_HEIGHT/2.0f;
bool firstMouse = true;

const float radius = 10.0f;
float currentFrame = 0.0f;
float deltatime = 0.0f;

glm::vec3 lightPos(0.0f,0.0f, 0.0f);

//data
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

int main(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"LearnOpenGL",NULL,NULL);
    if(window == NULL){
        std::cout<<"Failed to create window"<<'\n';
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"Failed to init GLAD"<<'\n';
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shaderProgram("shaderFiles/vertexCode3D_lightning.vs","shaderFiles/fragmentCode_lightning.fs");
    Shader shaderProgramLight("shaderFiles/vertexCode3D_lightsource.vs","shaderFiles/fragmentCode_lightshader.fs");

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::mat4 view;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f,0.0f,2.0f));
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    //Genereting texture
    unsigned int  deffuseMap = loadTexture("container2.png");
    unsigned int  specularMap = loadTexture("container2_specular.png");
    unsigned int  emissionMap = loadTexture("matrix.jpg");

    Material containerMat(deffuseMap,specularMap, emissionMap, 64.0f);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    while(!glfwWindowShouldClose(window)){
        processInput(window);
        glClearColor(0.1f,0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        deltatime = glfwGetTime() - currentFrame;
        currentFrame = glfwGetTime();

        //drawing cube with material
        shaderProgram.use();
        
        shaderProgram.setVec3("light.position", lightPos);
        shaderProgram.setVec3("viewPos", camera.GetPos());

        shaderProgram.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        shaderProgram.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        shaderProgram.setVec3("light.specular", 1.0f,1.0f,1.0f);
        shaderProgram.setFloat("loght.constant", 1.0f);
        shaderProgram.setFloat("loght.linear", 0.09f);
        shaderProgram.setFloat("loght.quadratic", 0.032f);
    
        shaderProgram.setMaterial(containerMat);

        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        model = glm::translate(model, glm::vec3(0.0f,0.0f,-2.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f,1.0f,0.0f));

        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f,100.0f);
        
        shaderProgram.setMat4("view",view);
        shaderProgram.setMat4("projection",projection);
        shaderProgram.setMat4("model",model);

        glActiveTexture(GL_TEXTURE0 + containerMat.diffuse);
        glBindTexture(GL_TEXTURE_2D,containerMat.diffuse);

        glActiveTexture(GL_TEXTURE0 + containerMat.specular);
        glBindTexture(GL_TEXTURE_2D, containerMat.specular);

        glActiveTexture(GL_TEXTURE0 + containerMat.emission);
        glBindTexture(GL_TEXTURE_2D, containerMat.emission);

        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::translate(model, glm::vec3(0.0f,0.0f,-1.0f));
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f,0.3f,0.5f));
            shaderProgram.setMat4("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0 ,36);
        }
        //draw light scource
        shaderProgramLight.use();
        shaderProgramLight.setMat4("projection", projection);
        shaderProgramLight.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        shaderProgramLight.setMat4("model",model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES,0,36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout<<"Hello World"<<std::endl;
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    shaderProgram.~Shader();
    shaderProgramLight.~Shader();
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window,int width, int heigh){
    glViewport(0,0,width,heigh);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window,true);

    if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS){
            camera.ProcessKeyboard(RIGHT, deltatime);
        }
        if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS){
            camera.ProcessKeyboard(LEFT, deltatime); 
        }
        if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS){
            camera.ProcessKeyboard(BACKWARD, deltatime);
        }
        if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS){
            camera.ProcessKeyboard(FORWARD, deltatime);
        }
        if(glfwGetKey(window, GLFW_KEY_SPACE)==GLFW_PRESS){
            camera.ProcessKeyboard(UP, deltatime); 
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS){
            camera.ProcessKeyboard(DOWN, deltatime); 
        }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

unsigned int loadTexture(const char* path){
    unsigned int texture;
    glGenTextures(1,&texture);

    //Texture loading
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path,&width,&height,&nrChannels,0);
    if(data){
        GLenum format;
        if(nrChannels == 1)
            format = GL_RED;
        if(nrChannels == 3)
            format = GL_RGB;
        if(nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }else{
        std::cout << "Failed to load texture" << std::endl;
        stbi_image_free(data);
    }

    return texture;
}