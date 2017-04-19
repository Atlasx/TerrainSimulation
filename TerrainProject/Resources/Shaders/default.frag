#version 330 core

in vec4 vertexColor;
in vec2 textureCoord;

out vec4 color;

//Textures
uniform sampler2D virtualMap;
uniform sampler2D textureAtlas;
uniform sampler2D perlinNoise;

//Other uniforms
uniform vec4 refTextureDimensions;
uniform vec2 virtualGridDimensions;
uniform float time;

//Gets the local texture coordinates from the virtual grid
void getLocalUV (in vec2 texCoords, out vec2 localTexCoords) {
    float texX = texCoords.x;
    float texY = texCoords.y;
    texX *= virtualGridDimensions.x;
    texY *= virtualGridDimensions.y;
    texX -= floor(texX);
    texY -= floor(texY);
    
    localTexCoords = vec2(texX, texY);
}

//Gets a pattern's ID from the virtual map
void getPatternID(in vec2 coords, out int ID) {
    //Pattern ID is taken from the R component of the virtual map
    ID = int(texture(virtualMap, coords).r * 255.f);
}

//Samples the atlas with UV at pattern with ID patternID
void getColorFromAtlas (in vec2 UV, in int patternID, out vec4 color) {
    float Y;
    Y = floor(float(patternID)/refTextureDimensions.x);
    float X;
    X = (float(patternID)/refTextureDimensions.x - Y) * refTextureDimensions.x;
    vec2 atlasCoords = vec2(X, Y);
    
    vec2 atlasUV;
    atlasUV = atlasCoords + UV;
    atlasUV.x /= refTextureDimensions.x;
    atlasUV.y /= refTextureDimensions.y;
    
    color = texture(textureAtlas, atlasUV);
}

//Simple perlin noise function to sample a random noise function
//Returns a value from 0-1
void perlin (in float x, in float y, out float ret) {
    ret = texture(perlinNoise, vec2(x, y)).r;
}

void main () {
    //Get localUV from virtual grid
    vec2 localUV;
    getLocalUV (textureCoord, localUV);
    
    //Get the pattern from the virtual map
    int patternID;
    getPatternID (textureCoord, patternID);
    
    //Get the corresponding color from the atlas
    getColorFromAtlas(localUV, patternID, color);
}
