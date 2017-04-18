#version 330 core

in vec4 vertexColor;
in vec2 textureCoord;

out vec4 color;

//Textures
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D virtualMap;

//Other uniforms
uniform vec2 refTexDim;
uniform vec2 virtualGridDimensions;

void getVirtualGridUV (in vec2 texCoords, out vec2 virtualTexCoords) {
    
}

void main () {
    float virtID = texture(virtualMap, textureCoord).x;
    color = texture(texture0, textureCoord)  * texture(texture1, textureCoord);
}
