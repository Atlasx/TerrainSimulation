//
//  Texture.cpp
//  TerrainProject
//
//  Created by Connor Douglas on 4/18/17.
//  Copyright © 2017 Connor Douglas. All rights reserved.
//

#include "Texture.hpp"

Texture::Texture (const GLchar* path) {
    //Load Texture
    int image_w, image_h;
    unsigned char* image = SOIL_load_image(path, &image_w, &image_h, 0, SOIL_LOAD_RGB);
    
    //Create Wall Texture
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_w, image_h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

void Texture::Use() {
    glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::Use(GLuint program, GLchar *uniform, GLuint textureSlot, int slot) {
    glActiveTexture(textureSlot);
    glBindTexture(GL_TEXTURE_2D, this->id);
    glUniform1i(glGetUniformLocation(program, uniform), slot);
}
