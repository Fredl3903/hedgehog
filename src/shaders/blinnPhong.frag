#version 430 core

layout(std140, binding = 0) uniform sceneUBO {
  mat4 modelMat;
  mat4 viewMat;
  mat4 projMat;
  vec3 camPos;
};
layout(std140, binding = 1) uniform materialUBO {
  float roughness;
  float metallic;
  vec3 emissive;
  float opacity;
  float rI;
};

uniform sampler2D modelTex1;
uniform sampler2D modelTex2;
uniform sampler2D depthMap;

in vec3 vertPos;
in vec3 vertNorm;
in vec3 vertColor;
in vec2 vertTex;
out vec4 FragColor;

void main() {

  vec3 endCol = vec3(0.0);

  vec4 texColor = texture(modelTex1, vertTex);
  
  vec3 baseDiffuse = vertColor * vec3(texColor);
  float ambient = 0.3f;
  endCol += vec3(ambient) * baseDiffuse;

  baseDiffuse = baseDiffuse * (1.0f - metal);

  vec3 lightPos = vec3(0.0f, 12.0f, 5.0f);
  vec3 lightDir = normalize(lightPos - vertPos);
  vec3 normal = normalize(vertNorm);

  float diff = max(dot(normal, lightDir), 0.0);
  
  vec3 specularColor = mix(vec3(0.05), vertColor, metal);
  float shiny = pow(1.0 - rough, 2.0) * 128.0;
  float spec = 0.0;

  vec3 viewDir = normalize(camPos - vertPos);
  vec3 halfwayDir = normalize(lightDir + viewDir);

  float NdotH = max(dot(normal, halfwayDir), 0.0);

  spec = pow(NdotH, shiny) * smoothstep(0.0, 0.2, diff);

  FragColor = vec4(vec3(ambient) * baseDiffuse + specularColor * spec + diffuseColor * diff + vec3(emission), 1.0f);

}