#version 430 core

// pair with blinnPhong.frag

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTex;

layout(std140, binding = 0) uniform geometryUBO {
  mat4 modelMat;
  mat4 viewMat;
  mat4 projMat;
  vec3 camPos;
};

out vec3 vertPos;
out vec3 vertNorm;
out vec3 vertColor;
out vec2 vertTex;

void main() {

  vertPos = vec3(modelMat * vec4(aPos, 1.0f));
  vertNorm = mat3(transpose(inverse(modelMat))) * aNorm;
  vertColor = aColor;
  vertTex = aTex;
  
  gl_Position = projMat * viewMat * vec4(vertPos, 1.0);

}