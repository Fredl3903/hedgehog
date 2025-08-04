#ifndef BASIC_TEX_LAYER
#define BASIC_TEX_LAYER

#include "basicLayer.hpp"

class basicTex3dLayer : public basic3dLayer {
  
  public:

    basicTex3dLayer() : basic3dLayer() {};    
    basicTex3dLayer(int zIndex) : basic3dLayer(zIndex) {
      setLayerShaders();
    }

    void setModelUniforms(std::shared_ptr<hhModel>& model, std::shared_ptr<hhMaterial> material) override;

  private:
    void setLayerShaders(const std::shared_ptr<hhShaderProgram>& newShaderProgram) override {}
    void setLayerShaders() {
      if (dimensionality == 3) {
        std::shared_ptr<hhShaderProgram> basicShaders = std::make_shared<hhShaderProgram>("basic3d.vert","basicTex.frag");
        layerShaders = basicShaders;
      }
    };
};

void basicTex3dLayer::setModelUniforms(std::shared_ptr<hhModel>& model, std::shared_ptr<hhMaterial> material) {
  if (!model) {
    std::cout << "null shared_ptr to model specified in setModelUniforms" << std::endl;
    return;
  }

  std::shared_ptr<hhTex> tex1 = material->getColorTex1();
  std::shared_ptr<hhTex> tex2 = material->getColorTex2();
  std::shared_ptr<hhTex> depth = material->getDepthMap();
  if (depth) {
    depth->bind(depth->getID());
  }
  if (tex1) {
    layerShaders->setUniform("modelTex1", *tex1);
  }
  if (tex2) {
    layerShaders->setUniform("modelTex2", *tex2);
  }
  
  glm::mat4* uModelMat = &model->modelMatrix;
  layerShaders->setUniform("modelMat", *uModelMat);
};

#endif