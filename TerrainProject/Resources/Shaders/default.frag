#version 330 core

in vec4 vertexColor;
in vec2 textureCoord;

out vec4 color;

//Other uniforms
uniform vec2 refTexDim;
uniform float virtualGD_X;
uniform float virtualGD_Y;
uniform vec2 virtualGridDimensions;

//Textures
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D virtualMap;

//Gets the local texture coordinates from the virtual grid
void getLocalUV (in vec2 texCoords, out vec2 localTexCoords) {
    float texX = texCoords.x;
    float texY = texCoords.y;
    texX *= virtualGD_X;
    texY *= virtualGD_Y;
    texX -= floor(texX);
    texY -= floor(texY);
    
    localTexCoords = vec2(texX, texY);
}

void main () {
    float virtID = texture(virtualMap, textureCoord).x;
    vec2 localUV;
    getLocalUV (textureCoord, localUV);
    

    //color = vec4(16, 16, 1.f, 1.f);
    color = vec4(localUV.x, localUV.y, 1.f, 1.f);
    //color = texture(texture0, textureCoord) * texture(texture1, textureCoord);
}
