//use "g++ main.cpp glad.c -Iinclude -lglfw -ldl -lGL -o main" to compile


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
void processInput(GLFWwindow* window, float* visibility);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

float yaw = -90.0f;
float pitch = 0.0f;

glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f,1.0f,0.0f);
float lastX = 400, lastY = 300;
bool firstMouse = true;
float cameraVelocity = 100.0f;

int main(){
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,600,"LearnOpenGL",NULL,NULL);
    if(window == NULL){
        std::cout<<"Failed to create window"<<'\n';
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwFocusWindow(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"Failed to init GLAD"<<'\n';
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shaderProgramRGB("shaderFiles/vertexCode3D.vs","shaderFiles/fragmentCodeRGB.fs");

     float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, 5*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);


    //Genereting texture
    unsigned int texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Texture loading
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("container.jpg",&width,&height,&nrChannels,0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Texture loading
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("awesomeface.png",&width,&height,&nrChannels,0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    shaderProgramRGB.use();
    glUniform1i(glGetUniformLocation(shaderProgramRGB.ID,"texture1"),0);
    shaderProgramRGB.setInt("texture2",1);

    float visibility = 0.5f;

    glm::vec3 cubePositions[] = {
        glm::vec3( 2.0f,  2.0f,  2.0f), 
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

    glm::mat4 view;
    view = glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),
                       glm::vec3(0.0f,0.0f,3.0f),
                       glm::vec3(0.0f,1.0f,0.0f));

    const float radius = 10.0f;

    

    float radians = glfwGetTime();
    float cameraSpeed = 5.0f;


    while(!glfwWindowShouldClose(window)){
        processInput(window, &visibility);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shaderProgramRGB.use();

        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        
        glm::mat4 projection    = glm::mat4(1.0f);
        
        float deltatime = glfwGetTime() - radians;
        cameraVelocity = cameraSpeed*deltatime;
        radians = glfwGetTime();

        //float camX = sin(glfwGetTime())*radius;
        //float camZ = cos(glfwGetTime())*radius;
        //view = glm::lookAt(glm::vec3(camX,0.0f,camZ), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
        
        model = glm::rotate(model, glm::radians(45*radians), glm::vec3(1.f, 1.0f, 1.0f));
        

        view  = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgramRGB.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgramRGB.ID, "view");
        
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc,1, GL_FALSE, glm::value_ptr(view));
        shaderProgramRGB.setMat4("projection",projection);

        int visibilityLoc = glGetUniformLocation(shaderProgramRGB.ID,"visibility");
        glUniform1f(visibilityLoc,visibility);

        glBindVertexArray(VAO);
        //Draw primary cube (True cude)
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Clone jutsu
        for(int i = 0; i < 10; i++){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,cubePositions[i]);
            if(i%3 == 0){
                float radians = glfwGetTime();
                model = glm::rotate(model, glm::radians(45*radians), glm::vec3(1.f, 1.0f, 1.0f));
            }
            else{
                float angle = 20*i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f,0.3f,0.5f));
            }
            shaderProgramRGB.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    std::cout<<"Hello World"<<std::endl;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1,&EBO);
    shaderProgramRGB.~Shader();
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window,int width, int heigh){
    glViewport(0,0,width,heigh);
}

void processInput(GLFWwindow* window, float* visibility){

    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window,true);
    if(glfwGetKey(window, GLFW_KEY_DOWN)==GLFW_PRESS){
        if(*visibility >=0.02f)
            *visibility-=0.02f;
    }
    if(glfwGetKey(window, GLFW_KEY_UP)==GLFW_PRESS){
        if(*visibility <= 0.98f)
            *visibility+=0.02f;
    }

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

    const float sensitivity = 1.f;
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

