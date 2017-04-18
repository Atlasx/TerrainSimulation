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

//Gets the local texture coordinates from the virtual grid
void getLocalUV (in vec2 texCoords, out vec2 localTexCoords) {
    localTexCoords = vec2(
                            texCoords.x / virtualGridDimensions.x,
                            texCoords.y / virtualGridDimensions.y
                            );
}

void main () {
    float virtID = texture(virtualMap, textureCoord).x;
    vec2 localUV;
    getLocalUV (textureCoord, localUV);
    
    color = vec4(localUV.x, localUV.y, 1.f, 1.f);
    //color = texture(texture0, textureCoord) * texture(texture1, textureCoord);
}
