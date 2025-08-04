#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTex;

out vec3 vertPos;
out vec3 vertColor;
out vec2 vertTex;

uniform mat4 modelMat;

void main() {

  vertPos = vec3(modelMat * vec4(aPos, 1.0f));
  vertColor = aColor;
  vertTex = aTex;
  
  gl_Position = vec4(vertPos.x, vertPos.y, 0.0, 1.0);

}