#version 430 core

uniform vec3 camPos;
uniform mat4 modelMat;

in vec3 vertPos;
in vec3 vertNorm;
in vec3 vertColor;
in vec2 vertTex;
out vec4 FragColor;


void main() {

  FragColor = vec4(vertColor, 1.0);

}