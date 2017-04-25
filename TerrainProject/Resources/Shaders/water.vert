#version 330 core

layout (location = 0) in vec3 position;

out vec4 vertexColor;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main () {
    vec3 pos = position;
    pos.y = 1.f;
    gl_Position = vec4(pos.xzy, 1.f);
    vertexColor = vec4(0.f, 0.f, 0.8f, 1.f);
}
