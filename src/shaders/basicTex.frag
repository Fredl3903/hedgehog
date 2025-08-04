#version 430 core

uniform sampler2D modelTex1;
uniform sampler2D modelTex2;
uniform sampler2D depthMap;

in vec3 vertPos;
in vec3 vertColor;
in vec2 vertTex;
out vec4 FragColor;

void main() {

  vec4 texColor = texture(modelTex1, vertTex);

  FragColor = vec4(vertColor * vec3(texColor), 1.0f);

}