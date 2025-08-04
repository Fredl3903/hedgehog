#version 430 core

layout(std140, binding = 0) uniform geometryUBO {
  mat4 modelMat;
  mat4 viewMat;
  mat4 projMat;
  vec3 camPos;
};

in vec3 vertPos;
in vec3 vertNorm;
in vec3 vertColor;
in vec2 vertTex;
out vec4 FragColor;

void main() {

  FragColor = vec4(vertColor, 1.0);

}