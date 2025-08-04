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

  vec3 specularColor = mix(vec3(0.05), vertColor, 0.5);
  float shiny = 64.0;
  float ambient = 0.2f;

  vec3 lightPos = vec3(0.0f, 10.0f, 5.0f);
  vec3 lightDir = normalize(lightPos - vertPos);
  vec3 normal = normalize(vertNorm);

  float diff = max(dot(normal, lightDir), 0.0);
  float spec = 0.0;

  vec3 viewDir = normalize(camPos - vertPos);
  vec3 halfwayDir = normalize(lightDir + viewDir);

  float NdotH = max(dot(normal, halfwayDir), 0.0);

  spec = pow(NdotH, shiny) * smoothstep(0.0, 0.2, diff);

  FragColor = vec4(vec3(ambient) * vertColor + specularColor * spec + vertColor * diff, 1.0f);

}