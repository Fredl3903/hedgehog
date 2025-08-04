#ifndef SCREEN_QUAD_LAYER
#define SCREEN_QUAD_LAYER

#include "../basic2dLayer.hpp"
#include "../../util/factory.hpp"

class screenQuadLayer : public basic2dLayer {
  
  public:

    std::shared_ptr<hhModel2d> quadModel = std::make_shared<hhModel2d>(GL_TRIANGLES);
    std::shared_ptr<hhMesh2d> quad;

    screenQuadLayer();    

    void setImage1(std::shared_ptr<hhImgTex> img) {       
      quadModel->material->setTexture1(img);
    }
    void setImage2(std::shared_ptr<hhImgTex> img) {       
      quadModel->material->setTexture2(img);
    }
    void setBaseCol(glm::vec3 col) { 
      quad->setCols(col);
    }
    void setDepthTex(std::shared_ptr<hhFBODepthTex> map) {
      quadModel->material->setDepthMap(map);
    }

    using hhLayer::setLayerShaders;
    void setLayerShaders() {
      std::shared_ptr<hhShaderProgram> basicShaders = std::make_shared<hhShaderProgram>("basic2d.vert","basicTex.frag");
      layerShaders = basicShaders;
    };
};


screenQuadLayer::screenQuadLayer() : basic2dLayer(0) {

  quad = std::make_shared<hhMesh2d>(
    hhMeshFactory::makeQuad(glm::vec2(-1.0, -1.0), glm::vec2(1.0, 1.0))
  );
  quadModel->addMesh(quad);
  addModel(quadModel);

  quadModel->setBuffers();

  std::shared_ptr<hhMaterial> quadMat = std::make_shared<hhMaterial>();
  quadModel->addMaterial(quadMat);

  setLayerShaders();
};


#endif