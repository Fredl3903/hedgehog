#ifndef SHADER_TEMPLATES
#define SHADER_TEMPLATES

#include <string>

namespace shaderBlocks {

// ************************** SETUP **************************
namespace setup {

  const std::string version = "#version 430 core";

  const std::string sceneUBO = R"(
    layout(std140, binding = 0) uniform sceneUBO {
    mat4 modelMat;
    mat4 viewMat;
    mat4 projMat;
    vec3 camPos;
  };
  )";

  const std::string materialUBO = R"(
    layout(std140, binding = 1) uniform materialUBO {
    float roughness;
    float metallic;
    vec3 emissive;
    float opacity;
    float rI;
  };
  )";

  const std::string useColorTexA = "uniform sampler2D modelTexA;";
  const std::string useColorTexB = "uniform sampler2D modelTexB;";
  const std::string useDepthMap = "uniform sampler2D depthMap;";
  const std::string useSpecularMap = "uniform sampler2D depthMap;";
  const std::string useNormalMap = "uniform sampler2D depthMap;";

  const std::string vertInPos = "layout(location = 0) in vec3 aPos;";
  const std::string vertInNorm = "layout(location = 1) in vec3 aNorm;";
  const std::string vertInCol = "layout(location = 2) in vec3 aColor;";
  const std::string vertInTex = "layout(location = 3) in vec2 aTex;";

  const std::string vertOutPos = "out vec3 vertPos;";
  const std::string vertOutNorm = "out vec3 vertNorm;";
  const std::string vertOutCol = "out vec3 vertColor;";
  const std::string vertOutTex = "out vec2 vertTex;";

  const std::string fragInPos = "in vec3 vertPos;";
  const std::string fragInNorm = "in vec3 vertNorm;";
  const std::string fragInCol = "in vec3 vertColor;";
  const std::string fragInTex = "in vec2 vertTex;";

  const std::string fragOutCol = "out vec4 FragColor;";

};

// *************************** VERTEX SHADER BODY ***************************
namespace vertShaderBody {

  const std::string basic2DVertMain = R"(
void main() {
  vertPos = vec3(modelMat * vec4(aPos, 1.0f));
  vertColor = aColor;
  )";
  const std::string glPos2D = R"(
  gl_Position = vec4(vertPos.x, vertPos.y, 0.0, 1.0);
};
)";

  const std::string basic3DVertMain = R"(
void main() {
  vertPos = vec3(modelMat * vec4(aPos, 1.0f));
  vertNorm = mat3(transpose(inverse(modelMat))) * aNorm;
  vertColor = aColor;
  )";
  const std::string glPos3D = R"(
  gl_Position = projMat * viewMat * vec4(vertPos, 1.0);
};
  )";

  const std::string setVertTex = "  vertTex = aTex;";

};


// ************************** FRAGMENT SHADER BODY **************************
namespace fragShaderBody {

  const std::string fragShaderMainStart = R"( 
void main() {

  vec3 endCol = vec3(0.0);

  )";
  
  const std::string baseDiffuse = R"(
  vec3 baseDiffuse = vertColor;
  endCol = baseDiffuse;
  )";
  const std::string baseDiffuseTex = R"(
  vec3 baseDiffuse = vertColor * vec3(texColor);
  endCol = baseDiffuse;
  )";

  const std::string ambient = R"(
  float ambient = 0.3f;
  endCol *= ambient;
  )";

  const std::string baseDiffuseMetallic = "  baseDiffuse = baseDiffuse * (1.0f - metal);";

  const std::string diffUnlit = " float diff = 1.0";
  const std::string diffLit = R"(
  vec3 lightPos = vec3(0.0f, 10.0f, 5.0f);
  vec3 lightDir = normalize(lightPos - vertPos);
  vec3 normal = normalize(vertNorm);

  float diff = max(dot(normal, lightDir), 0.0);
  )";

  const std::string diffuse = "  endCol += diff * baseDiffuse";

  const std::string specular = R"(
  vec3 specularColor = mix(vec3(0.05), vertColor, metal);
  float shiny = pow(1.0 - rough, 2.0) * 128.0;
  float spec = 0.0;

  vec3 viewDir = normalize(camPos - vertPos);
  vec3 halfwayDir = normalize(lightDir + viewDir);

  float NdotH = max(dot(normal, halfwayDir), 0.0);

  spec = pow(NdotH, shiny) * smoothstep(0.0, 0.2, diff);

  endCol += specularColor * spec;
  )";

  const std::string emission = "  endCol += emissive;";

  const std::string fragShaderMainEnd = R"(
  FragColor = vec4(endCol, 1.0);
};  
  )";


};

};


#endif