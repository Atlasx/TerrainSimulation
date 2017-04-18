//
//  Texture.hpp
//  TerrainProject
//
//  Created by Connor Douglas on 4/18/17.
//  Copyright © 2017 Connor Douglas. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <SOIL.h>

#include <GL/glew.h>

class Texture
{
public:
    GLuint id;
    Texture (const GLchar* path);
    void Use();
    void Use(GLuint program, GLchar* uniform, GLuint textureSlot, int slot);
};

#endif /* Texture_hpp */
