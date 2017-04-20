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
uniform vec2 virtualMapDimensions;
uniform vec2 virtualGridDimensions;
uniform float time;

//Simple perlin noise function to sample a random noise function
//Returns a value from 0-1
void perlin (in float x, in float y, out float ret) {
    ret = texture(perlinNoise, vec2(x, y)).r;
}

//A random number generator, taken from the web
//https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
void rand(in vec2 co, out float ret){
    ret = fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
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

//Gets a pattern's ID from the virtual map using UV coordinates
void getPatternID(in vec2 coords, out int ID) {
    //Pattern ID is taken from the R component of the virtual map
    ID = int(texture(virtualMap, coords).r * 255.f);
}

//Gets a pattern ID from virtual map coordinates
void getVirtMapPatternID(in vec2 coords, out int ID) {
    coords.x /= virtualMapDimensions.x;
    coords.y /= virtualMapDimensions.y;
    
    ID = int(texture(virtualMap, coords).r * 255.f);
}

//Gets the current virtual map coordinates
void getVirtualMapCoords (in vec2 virtualGridUV, out vec2 coords) {
    float X;
    float Y;
    X = floor(virtualGridUV.x * virtualMapDimensions.x);
    Y = floor(virtualGridUV.y * virtualMapDimensions.y);
    coords = vec2(X, Y);
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

//Dithers the reference texture
void getDitheredPatternID (in vec2 UV, out int ID) {
    //Sample nearby virtual MAP spaces and see if there is any change happening.
    
    vec2 virtMapCoords;
    getVirtualMapCoords (UV, virtMapCoords);
    
    vec2 virtGridCoords;
    getVirtualGridCoord(virtGridCoords);
    
    int centerID;
    getVirtMapPatternID(virtMapCoords, centerID);
    
    //Left
    vec2 leftCoords;
    leftCoords = virtMapCoords;
    leftCoords.x -= 1.f;
    leftCoords.x = mod(leftCoords.x, virtualMapDimensions.x);
    
    int leftID;
    getVirtMapPatternID(leftCoords, leftID);
    
    //Right
    vec2 rightCoords;
    rightCoords = virtMapCoords;
    rightCoords.x += 1.f;
    rightCoords.x = mod(rightCoords.x, virtualMapDimensions.x);
    
    int rightID;
    getVirtMapPatternID(rightCoords, rightID);
    
    //Up
    vec2 upCoords;
    upCoords = virtMapCoords;
    upCoords.y += 1.f;
    upCoords.y = mod(upCoords.y, virtualMapDimensions.y);
    
    int upID;
    getVirtMapPatternID(upCoords, upID);
    
    //Down
    vec2 downCoords;
    downCoords = virtMapCoords;
    downCoords.y -= 1.f;
    downCoords.y = mod(downCoords.y, virtualMapDimensions.y);
    
    int downID;
    getVirtMapPatternID(downCoords, downID);
    
    //Compare IDs and lerp between different IDs
    vec4 ids;
    ids.x = leftID;
    ids.y = rightID;
    ids.z = upID;
    ids.w = downID;
    
    //Each ID has the same probability
    float r;
    rand(virtGridCoords, r);
    r = floor(abs(r)*2.f);
    ID = int(floor(r)) * centerID + int(floor(1-r))*leftID;
}

void main () {
    //Get localUV from virtual grid
    vec2 localUV;
    getLocalUV (textureCoord, localUV);
    
    //Get the pattern from the virtual map
    int patternID;
    getPatternID (textureCoord, patternID);
    getDitheredPatternID(textureCoord, patternID);
    
    //Randomly flip the UV
    randomUVFlip(localUV);
    
    //Get the corresponding color from the atlas
    getColorFromAtlas(localUV, patternID, color);
}
