#version 330 core

// pair with blankFrag.frag

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTex;

out vec3 vertColor;

uniform vec3 camPos;

void main() {
  vec3 worldPos = vec3(modelMat * vec4(aPos, 1.0f));
  
  vertColor = aColor;
  gl_Position = projMat * viewMat * vec4(worldPos, 1.0);
}