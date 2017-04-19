//
//  main.cpp
//  TerrainProject
//
//  Created by Connor Douglas on 4/11/17.
//  Copyright © 2017 Connor Douglas. All rights reserved.
//

#include <iostream>
#include <map>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>

#include "Shader.hpp"
#include "Texture.hpp"
#include "SimpleTime.hpp"

void UpdateFPS(GLFWwindow *window);
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void CursorCallback(GLFWwindow *window, double xpos, double ypos);

int SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720;

int refTexX, refTexY, refTexWidth, refTexHeight;
int virtualGridWidth, virtualGridHeight;

struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

//Simple global time for fps and deltaTime calc
SimpleTime g_time;

int main(int argc, const char * argv[]) {
 
    //Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    
    //Create GLFW Window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Terrain Simulation (0.00 fps)", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "ERROR::GLFW: Failed to create a GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    //Disable vsync
    glfwSwapInterval(0);
    
    //Set callbacks
    glfwSetKeyCallback(window, KeyCallback);
    
    //Print OpenGL Version
    std::cout << "DEBUG: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    
    //Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR::GLEW: Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    //Time setup
    g_time.deltaTime = 0.0;
    g_time.startTime = glfwGetTime();
    g_time.prevFrameTime = glfwGetTime();
    g_time.frames = 0;
    g_time.fps = 0.f;
    
    //Basic Framebuffer Setup
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    //Set Clear Color
    glClearColor(0.f, 0.f, 0.f, 1.f);
    
    //Load default shader
    Shader defaultShader ("./Resources/Shaders/default.vert", "./Resources/Shaders/default.frag");
    
    //Basic plane
    GLfloat vertices[] = {
        1.f, 1.f, 0.f,
        1.f, -1.f, 0.f,
        -1.f, 1.f, 0.f,
        1.f, -1.f, 0.f,
        -1.f, -1.f, 0.f,
        -1.f, 1.f, 0.f
    };
    
    //Test Colors
    GLfloat colors[] {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,
        0.f, 1.f, 0.f
    };
    
    //Test UV coords
    GLfloat textures[] = {
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        1.f, 0.f,
        0.f, 0.f,
        0.f, 1.f
    };
    
    //Generate Array Object
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    //Generate Vertex Buffers
    GLuint vertexVBO;
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //Vertex Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //Generate Color Buffers
    GLuint colorVBO;
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    
    //Color Attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    //Generate UV buffers
    GLuint textureVBO;
    glGenBuffers(1, &textureVBO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textures), textures, GL_STATIC_DRAW);
    
    //Texture Attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
    
    //Time uniform
    GLuint timeID = glGetUniformLocation(defaultShader.Program, "time");
    if (timeID == -1) {
        std::cout << "Failed to find time" << std::endl;
    }
    
    //Load Textures
    Texture wallTex ("Resources/Images/wall.jpg", GL_TEXTURE_2D);
    Texture refTex ("Resources/Images/refTexture.png", GL_TEXTURE_2D);
    Texture virtMap ("Resources/Images/IslandTest.png", GL_TEXTURE_2D);
    
    //Set reference texture dimensions
    refTexX = 4;
    refTexY = 4;
    refTexWidth = 16;
    refTexHeight = 16;
    virtualGridWidth = 16;
    virtualGridHeight = 16;
    
    //Reference texture dimensions
    glUseProgram(defaultShader.Program);
    glUniform4f(glGetUniformLocation(defaultShader.Program, "refTextureDimensions"), (GLfloat)refTexX, (GLfloat)refTexY, (GLfloat)refTexWidth, (GLfloat)refTexHeight);
    glUniform2f(glGetUniformLocation(defaultShader.Program, "virtualGridDimensions"), (GLfloat)virtualGridWidth, (GLfloat)virtualGridHeight);
    
    refTex.Use(defaultShader.Program, "textureAtlas", GL_TEXTURE0, 0, GL_TEXTURE_2D);
    virtMap.Use(defaultShader.Program, "virtualMap", GL_TEXTURE1, 1, GL_TEXTURE_2D);
    
    //Main Loop
    while (!glfwWindowShouldClose(window)) {
        
        //FPS
        UpdateFPS(window);
        
        //Input Events
        glfwPollEvents();
        
        //Clear Buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //Use default shader
        defaultShader.Use();
        
        //Time uniform
        glUniform1f(timeID, (GLfloat)glfwGetTime());
        
        //Draw plane
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        
        //Swap Buffers
        glfwSwapBuffers(window);
    }
    
    //Cleanup
    glfwTerminate();
    return 0;
}

void UpdateFPS(GLFWwindow *window) {
    g_time.frames++;
    g_time.deltaTime = glfwGetTime() - g_time.prevFrameTime;
    if (glfwGetTime() - g_time.startTime > 1.0) {
        char title [256];
        title [255] = '\0';
        g_time.fps = (double)g_time.frames / (glfwGetTime() - g_time.startTime);
        g_time.frames = 0;
        snprintf(title, 255, "Terrain Simulation (%.2f fps - %.2f ms)", g_time.fps, g_time.deltaTime * 1000);
        glfwSetWindowTitle(window, title);
        g_time.startTime = glfwGetTime();
    }
    g_time.prevFrameTime = glfwGetTime();
}

//TODO Implement First-Person movement
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_W:
                glClearColor(1.f, 0.f, 0.f, 1.f);
                break;
            case GLFW_KEY_A:
                glClearColor(0.6f, 0.1f, 0.3f, 1.f);
                break;
            case GLFW_KEY_S:
                glClearColor(0.1f, 0.8f, 0.2f, 1.f);
                break;
            case GLFW_KEY_D:
                glClearColor(0.f, 0.f, 4.f, 1.f);
                break;
        }
    }
}

void CursorCallback(GLFWwindow *window, double xpos, double ypos) {
    //TODO Implement First-Person movement
}
