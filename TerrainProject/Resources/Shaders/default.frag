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

//Simple perlin noise function to sample a random noise function
//Returns a value from 0-1
void perlin (in float x, in float y, out float ret) {
    ret = texture(perlinNoise, vec2(x, y)).r;
}

//Returns the current virtual grid coordinate
void getVirtualGridCoord (out vec2 coord) {
    float X;
    float Y;
    X = floor(textureCoord.x * virtualGridDimensions.x);
    Y = floor(textureCoord.y * virtualGridDimensions.y);
    coord = vec2(X, Y);
}

//Based on perlin noise, randomly flips the localUV to get more random textures
//It just samples the global virtual grid UV for the perlin value
void randomUVFlip (inout vec2 UV) {
    float flipX;
    float flipY;
    
    //First Octave
    vec2 vgCoord;
    getVirtualGridCoord(vgCoord);
    vgCoord /= virtualGridDimensions.x;
    
    perlin(vgCoord.x, vgCoord.y, flipX);
    perlin(vgCoord.y, vgCoord.x, flipY);
    
    //Second Octave
    vgCoord *= 8.f;
    
    float flipX_o2;
    float flipY_o2;
    perlin(vgCoord.x, vgCoord.y, flipX_o2);
    perlin(vgCoord.y, vgCoord.x, flipY_o2);
    
    flipX = (flipX + flipX_o2) / 2.f;
    flipY = (flipY + flipY_o2) / 2.f;
    
    //Apply flip to UV
    flipX = floor(flipX * 2.f);
    flipY = floor(flipY * 2.f);
    UV.x = (flipX * UV.x) + ((1-flipX) * (1-UV.x));
    UV.y = (flipY * UV.y) + ((1-flipY) * (1-UV.y));
}

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

void main () {
    //Get localUV from virtual grid
    vec2 localUV;
    getLocalUV (textureCoord, localUV);
    
    //Get the pattern from the virtual map
    int patternID;
    getPatternID (textureCoord, patternID);
    
    //Randomly flip the UV
    randomUVFlip(localUV);
    
    //Get the corresponding color from the atlas
    getColorFromAtlas(localUV, patternID, color);
    /*
    vec2 vgrid;
    getVirtualGridCoord(vgrid);
    vgrid /= 16.f;
    color = vec4(vgrid, 0.f, 1.f);
     */
}
