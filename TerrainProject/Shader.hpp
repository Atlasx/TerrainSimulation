//
//  Shader.hpp
//  TerrainProject
//
//  Created by Connor Douglas on 4/11/17.
//  Copyright © 2017 Connor Douglas. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
    GLuint Program;
    Shader (const GLchar* vertPath, const GLchar* fragPath);
    void Use();
private:
    GLuint LoadShader (GLenum type, const GLchar* path);
};

#endif /* Shader_hpp */
