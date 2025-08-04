#version 430 core

uniform vec3 camPos;
uniform mat4 modelMat;

uniform sampler2D modelTex1;
uniform sampler2D modelTex2;
uniform sampler2D depthMap;

in vec3 vertPos;
in vec3 vertColor;
in vec2 vertTex;
out vec4 FragColor;

void main() {

  vec4 baseColor = texture(modelTex1, vertTex);
  vec4 overlay = texture(modelTex2, vertTex);

  // Alpha blending
  float alpha = overlay.a;
  FragColor = mix(baseColor, overlay, alpha);
}