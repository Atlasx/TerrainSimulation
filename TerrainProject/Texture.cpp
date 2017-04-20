//
//  Texture.cpp
//  TerrainProject
//
//  Created by Connor Douglas on 4/18/17.
//  Copyright © 2017 Connor Douglas. All rights reserved.
//

#include "Texture.hpp"

Texture::Texture (const GLchar* path, GLuint texture_type) {
    //Load Texture
    int image_w, image_h;
    unsigned char* image = SOIL_load_image(path, &image_w, &image_h, 0, SOIL_LOAD_RGB);
    
    //Create Wall Texture
    glGenTextures(1, &this->id);
    glBindTexture(texture_type, this->id);
    //Currently uses NEAREST to prevent smoothing from mipmapping
    glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexImage2D(texture_type, 0, GL_RGB, image_w, image_h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(texture_type);
    SOIL_free_image_data(image);
    glBindTexture(texture_type, 0);
    
}

void Texture::Use() {
    glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::Use(GLuint program, GLchar *uniform, GLuint textureSlot, int slot, GLuint texture_type) {
    glActiveTexture(textureSlot);
    glBindTexture(texture_type, this->id);
    glUniform1i(glGetUniformLocation(program, uniform), slot);
}
