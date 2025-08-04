#ifndef BASIC_2D_LAYER
#define BASIC_2D_LAYER

#include "layer.hpp"

class basic2dLayer : public hhLayer {
  
  public:

    basic2dLayer() : hhLayer() {};    
    basic2dLayer(int zIndex) : hhLayer(2, zIndex) {
      setLayerShaders(true);
    }

    virtual void renderLayer() override;

    void setModelUniforms(std::shared_ptr<hhModel>& model, std::shared_ptr<hhMaterial> material) override;

    using hhLayer::setLayerShaders;

  private:
    void setLayerShaders(bool lighting) {
      std::shared_ptr<hhShaderProgram> basicShaders = std::make_shared<hhShaderProgram>("basic2d.vert","basicTex.frag");
      layerShaders = basicShaders;
    };
};

void basic2dLayer::renderLayer() {  
  
  // Check shaders and camera are valid before proceeding

  if (!layerShaders) {
    std::cout << "null ptr to shaders in renderLayer()" << std::endl;
    return; // Exit early if shader is null
  } else {
    layerShaders->use();
  }

  renderAllModels();
};

void basic2dLayer::setModelUniforms(std::shared_ptr<hhModel>& model, std::shared_ptr<hhMaterial> material) {
  if (!model) {
    std::cout << "null shared_ptr to model specified in setModelUniforms" << std::endl;
    return;
  }

  std::shared_ptr<hhTex> tex1 = material->getColorTex1();
  std::shared_ptr<hhTex> tex2 = material->getColorTex2();
  std::shared_ptr<hhTex> depthMap = material->getDepthMap();

  if (tex1) {
    layerShaders->setUniform("modelTex1", *tex1);
  }
  if (tex2) {
    layerShaders->setUniform("modelTex2", *tex2);
  }
  if (depthMap) {
    layerShaders->setUniform("depthMap", *depthMap);
  }

  glm::mat4* uModelMat = &model->modelMatrix;
  layerShaders->setUniform("modelMat", *uModelMat);
}

#endif