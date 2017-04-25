#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

uniform float time;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform sampler2D heightMap;

out vec4 vertexColor;
out vec2 textureCoord;

void main () {
    vec3 pos = position;
    pos.y = texture(heightMap, (pos.xz+1.f)/2.f).x/2.f;
    gl_Position = proj * view * model * transform * vec4(pos.xzy, 1.f);
    textureCoord = vec2((position.x + 1.f)/2.f, (position.z+1.f)/2.f);//vec2(texCoord.x, 1.f - texCoord.y);
    
    //Wavey color visual test
    //vertexColor = vec4(abs(sin(time + color.x)), abs(sin(time + color.y)), abs(sin(time + color.z)), 1.f) * vec4(texCoord.x, texCoord.y, abs(sin(time*2.5f)), 1.f);
}
