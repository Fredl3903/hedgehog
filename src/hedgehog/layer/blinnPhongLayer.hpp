#ifndef BP_LAYER
#define BP_LAYER

#include "basicLayer.hpp"

class blinnPhongLayer : public basic3dLayer {
  
  public:

    blinnPhongLayer();    
    blinnPhongLayer(int zIndex);

    void setModelUniforms(std::shared_ptr<hhModel>& model, std::shared_ptr<hhMaterial> material) override;

  private:
    void setLayerShaders(const std::shared_ptr<hhShaderProgram>& newShaderProgram) override;
    void setLayerShaders();
};

blinnPhongLayer::blinnPhongLayer() : basic3dLayer() {};    
blinnPhongLayer::blinnPhongLayer(int zIndex) : basic3dLayer(zIndex) {
  setLayerShaders();
}

void blinnPhongLayer::setLayerShaders(const std::shared_ptr<hhShaderProgram>& newShaderProgram) {}
void blinnPhongLayer::setLayerShaders() {
  std::shared_ptr<hhShaderProgram> blinnPhong = std::make_shared<hhShaderProgram>("blinnPhong.vert","blinnPhong.frag");
  layerShaders = blinnPhong;
};


void blinnPhongLayer::setModelUniforms(std::shared_ptr<hhModel>& model, std::shared_ptr<hhMaterial> material) {
  if (!model) {
    std::cout << "null shared_ptr to model specified in setModelUniforms" << std::endl;
    return;
  }

  // set up material uniforms
  layerShaders->setUniform("metal", material->getMetallic()); // 0
  layerShaders->setUniform("rough", material->getRoughness()); // 1
  layerShaders->setUniform("emission", material->getEmission()); // 0, 0, 0

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
}

#endif

