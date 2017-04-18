//
//  Shader.cpp
//  TerrainProject
//
//  This is a combination of the Shader class from learnopengl.com
//  and my own code. Mainly separating the LoadShader method.
//
//  Created by Connor Douglas on 4/11/17.
//  Copyright © 2017 Connor Douglas. All rights reserved.
//

#include "Shader.hpp"

//Loads in vertex and fragment shaders from given paths
Shader::Shader (const GLchar* vertPath, const GLchar* fragPath) {
    
    //Load in shaders
    GLuint vertex = LoadShader(GL_VERTEX_SHADER, vertPath);
    GLuint fragment = LoadShader(GL_FRAGMENT_SHADER, fragPath);
    
    //Create shader program
    this->Program = glCreateProgram();
    
    //Attach vertex and fragment shaders
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    
    //Error helpers
    GLint success;
    GLchar infoLog[512];
    
    //Link shaders
    glLinkProgram(this->Program);
    
    //Error checking
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cout << "ERROR::OPENGL::SHADER: Failed to link program.\n" << infoLog << std::endl;
    }
    
    //Cleanup shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

//Loads a shader of type from path
GLuint Shader::LoadShader(GLenum type, const GLchar* path) {
    
    //Create new shader of type
    GLuint shader = glCreateShader(type);
    
    //Setup for reading file
    std::string code;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    //Read file and catch failure
    try
    {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::OPENGL::SHADER: Failed to read shader file." << std::endl;
    }
    
    //Create a const of the read file
    const GLchar* shaderCode = code.c_str();
    
    //Error helpers
    GLint success;
    GLchar infoLog[512];
    
    //Compile shader
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    
    //Error checking
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::OPENGL::SHADER: Compilation failed.\n" << infoLog << std::endl;
    }
    
    //Return successfully created shader
    return shader;
}

//Uses the local shader program
void Shader::Use() {
    glUseProgram(this->Program);
}
