#ifndef SHADER_MANAGER
#define SHADER_MANAGER

#include <memory>
#include "../shader/shaderProgram.hpp"
#include "../layer/layer.hpp"

class hhShaderManager {

  public:

    std::unordered_map<ShaderSignature, size_t> shaderSignatureMap;
    std::vector<hhShaderProgram> shaders;

    hhShaderManager() {}

    void compareLayer(std::shared_ptr<hhLayer> newLayer);
    void createShader(const ShaderSignature& sign);

    void layerSetup(size_t layerShaderID);

};

void hhShaderManager::compareLayer(std::shared_ptr<hhLayer> newLayer) {

  ShaderSignature sign = newLayer->signature;

  auto it = shaderSignatureMap.find(sign);
  if (it != shaderSignatureMap.end()) {
    newLayer->shaderID = it->second;
  } else {
    createShader(sign);
    size_t shaderID = shaders.size() - 1;
    shaderSignatureMap[sign] = shaderID;
    newLayer->shaderID = shaderID;
  }

}

void hhShaderManager::createShader(const ShaderSignature& sign) {

  hhShaderProgram newShader;
  newShader.buildFromSignature(sign);
  shaders.push_back(newShader);

}

void hhShaderManager::layerSetup(size_t layerShaderID) {
  shaders[layerShaderID].use();
}


#endif