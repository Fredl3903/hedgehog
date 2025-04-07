#version 330 core

uniform vec3 uCamPos;
uniform vec2 uRes;

in vec4 color;
out vec4 fragCol;

void main() {

    fragCol = color;

}