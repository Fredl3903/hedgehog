#version 330 core
    
uniform mat4 viewMat;
uniform mat4 modelMat;
uniform mat4 projMat;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec3 col;
layout(location = 3) in vec3 tex;

out vec4 fragCol;

void main() {

  fragCol = vec4(col, 1.0);

  gl_Position = projMat * viewMat * modelMat * vec4(pos, 1.0);

}