#ifndef GL_MANAGER
#define GL_MANAGER

#include "../buffer/UBO.hpp"
#include "../layer/layer.hpp"

struct sceneData {
  glm::mat4 modelMat;
  glm::mat4 viewMat;
  glm::mat4 projMat;
  glm::vec3 camPos;
  float _pad0;
};
struct materialData {
    float roughness = 0.5;                    // offset 0
    float metallic = 0.5;                     // offset 4
    float _pad0[2];                           // offset 8–15

    glm::vec3 emissiveCol = glm::vec3(0.0);   // offset 16
    float opacity = 1.0;                      // offset 28

    float refractiveIndex = 1.0;              // offset 32
    float _pad1[3];                           // offset 36–47
};


class hhManager {

  public:

    hhUBO<sceneData> sceneUBO = hhUBO<sceneData>(0);
    hhUBO<materialData> materialUBO = hhUBO<materialData>(1);

    std::shared_ptr<hhTex2d> colorTexA = std::make_shared<hhTex2d>(1, 1);
    std::shared_ptr<hhTex2d> colorTexB = std::make_shared<hhTex2d>(1, 1);;
    std::shared_ptr<hhTex2d> depthTex = std::make_shared<hhTex2d>(1, 1);;

    hhManager();

};

#endif