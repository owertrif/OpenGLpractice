#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "math.h"
#include "shader/shader.h"
#include "std_img/stb_image_implementation.h"

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window,int width, int heigh);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

float yaw = -90.0f;
float pitch = 0.0f;

glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f,1.0f,0.0f);
float lastX = 400, lastY = 300;
bool firstMouse = true;
float cameraSpeed = 5.0f;
float cameraVelocity = 0.0f;

const float radius = 10.0f;
float currentFrame = glfwGetTime();
float deltatime = glfwGetTime() - currentFrame;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
glm::vec3 lightPos(1.2f,0.0f, 2.0f);

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

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::mat4 view;
    view = glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),
                       glm::vec3(0.0f,0.0f,3.0f),
                       glm::vec3(0.0f,1.0f,0.0f));

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    
    while(!glfwWindowShouldClose(window)){
        processInput(window);
        glClearColor(0.1f,0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        shaderProgram.use();
        
        shaderProgram.setVec3("light.position", lightPos);
        shaderProgram.setVec3("viewPos", cameraPos);

        shaderProgram.setVec3("light.ambient", 1.0f,1.0f,1.0f);
        shaderProgram.setVec3("light.diffuse", 1.0f,1.0f,1.0f);
        shaderProgram.setVec3("light.specular", 1.0f,1.0f,1.0f);

        shaderProgram.setVec3("material.ambient", 0.24725f,0.1995f,0.0745f);
        shaderProgram.setVec3("material.diffuse", 0.75164f,0.60648f,0.22648f);
        shaderProgram.setVec3("material.specular", 0.628281f,0.555802f,0.366065f);
        shaderProgram.setFloat("material.shininess", 64.0f);

        deltatime = glfwGetTime() - currentFrame;
        currentFrame = glfwGetTime();
        cameraVelocity = cameraSpeed*deltatime;

        model = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f,100.0f);
        
        shaderProgram.setMat4("view",view);
        shaderProgram.setMat4("projection",projection);
        shaderProgram.setMat4("model",model);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0 ,36);

        shaderProgramLight.use();
        
        model = glm::mat4(1.0f);
        model = glm::translate(model,lightPos);
        model = glm::scale(model,glm::vec3(0.2f));
        shaderProgramLight.setMat4("projection",projection);
        shaderProgramLight.setMat4("view",view);
        shaderProgramLight.setMat4("model",model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0 ,36);

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
            cameraPos += glm::normalize(glm::cross(cameraFront,cameraUp))*cameraVelocity; 
        }
        if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS){
            cameraPos -= glm::normalize(glm::cross(cameraFront,cameraUp))*cameraVelocity; 
        }
        if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS){
            cameraPos-= cameraVelocity*cameraFront;
        }
        if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS){
            cameraPos+= cameraVelocity*cameraFront;
        }
        if(glfwGetKey(window, GLFW_KEY_SPACE)==GLFW_PRESS){
            cameraPos += glm::cross(glm::normalize(glm::cross(cameraFront,cameraUp)),cameraFront)*cameraVelocity;  
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS){
            cameraPos -= glm::cross(glm::normalize(glm::cross(cameraFront,cameraUp)),cameraFront)*cameraVelocity; 
        }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset*=sensitivity;
    yoffset*=sensitivity;

    yaw += xoffset;
    pitch -= yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
    
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw))*cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw))*cos(glm::radians(pitch));

    cameraFront = glm::normalize(direction);
}