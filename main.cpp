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
void processInput(GLFWwindow* window, float* visibility, glm::mat4* trans);

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

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"Failed to init GLAD"<<'\n';
        return -1;
    }

    Shader shaderProgramRGB("shaderFiles/vertexCode.vs","shaderFiles/fragmentCodeRGB.fs");

     

     float vertices[] = {
        //cords             //colors          //texturecords
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// bottom left
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f// top left 
    };

    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);


    //Genereting texture
    unsigned int texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.0f,0.0f,1.0f));
    trans = glm::rotate(trans, glm::radians(0.0f), glm::vec3(0.0f,0.0f,1.0f));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f,0.0f,1.0f));
    model = glm::rotate(model, glm::radians(-55.0f),glm::vec3(1.0f,0.0f,0.0f));
    
    glm::mat4 proj = glm::perspective(glm::radians(45.0f),(float)width/(float)height,0.1f,100.f);


    while(!glfwWindowShouldClose(window)){
        processInput(window, &visibility, &trans);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shaderProgramRGB.use();

        int visibilityLoc = glGetUniformLocation(shaderProgramRGB.ID,"visibility");
        glUniform1f(visibilityLoc,visibility);

        int transformLoc = glGetUniformLocation(shaderProgramRGB.ID, "transform");
        glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(trans));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

void processInput(GLFWwindow* window, float* visibility, glm::mat4* trans){

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
    if(glfwGetKey(window, GLFW_KEY_LEFT)==GLFW_PRESS){
        *trans = glm::rotate(*trans, glm::radians(1.0f), glm::vec3(0.0f,0.0f,1.0f));
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT)==GLFW_PRESS){
        *trans = glm::rotate(*trans, glm::radians(-1.0f), glm::vec3(0.0f,0.0f,1.0f));
    }
}

