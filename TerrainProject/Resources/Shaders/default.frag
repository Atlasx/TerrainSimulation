#version 330 core

in vec4 vertexColor;
in vec2 textureCoord;

out vec4 color;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main () {
    color = texture(texture0, textureCoord)  * texture(texture1, textureCoord);
}
