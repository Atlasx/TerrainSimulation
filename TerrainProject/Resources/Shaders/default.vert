#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

uniform float time;

out vec4 vertexColor;
out vec2 textureCoord;

void main () {
    gl_Position = vec4(position, 1.f);
    textureCoord = vec2(texCoord.x, 1.f - texCoord.y);
    
    //Wavey color visual test
    vertexColor = vec4(abs(sin(time + color.x)), abs(sin(time + color.y)), abs(sin(time + color.z)), 1.f) * vec4(texCoord.x, texCoord.y, abs(sin(time*2.5f)), 1.f);
}
