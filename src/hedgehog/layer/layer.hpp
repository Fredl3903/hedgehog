#ifndef LAYER
#define LAYER

#include <memory>

#include <C:/OpenGL/GLEW/include/GL/glew.h>
#include <C:/OpenGL/glm/glm/gtc/type_ptr.hpp>
#include <unordered_set>

#include "../model/model2d.hpp"
#include "../model/model3d.hpp"
#include "../cam/camera.hpp"
#include "../shader/shaderProgram.hpp"

enum camera {
  FIXED, FREE
};

class hhLayer {
  
  public:
    int numModels = 0;
    unsigned int dimensionality;
    camera camType = FREE;
    
    size_t shaderID;
    ShaderSignature signature;
    std::vector<std::shared_ptr<hhModel>> models;

    unsigned int order = 0;

    hhLayer() : order(0), dimensionality(3) {};    
    hhLayer(unsigned int dimension, int zIndex) : order(zIndex), dimensionality(dimension) {}
    // move constructor
    hhLayer(hhLayer&& other) noexcept
      : numModels(other.numModels),
        models(std::move(other.models)),
        shaderID(other.shaderID),
        order(other.order) {}

    virtual void addModel(const std::shared_ptr<hhModel>& newModel);

    void setAllModelBuffers();

    virtual void renderLayer() = 0;
    virtual void renderAllModels();
        
    unsigned int getZIndex() const { return order; }
};


// ************************************************************************************************


void hhLayer::addModel(const std::shared_ptr<hhModel>& newModel) {        
  
  if (newModel->dimensionality != dimensionality) {
    std::cout << "mismatching dimensionality in hhLayer::addModel call" << std::endl;
    return;
  }
  
  models.push_back(newModel);
  numModels += 1;
}

void hhLayer::setAllModelBuffers() {
  for (std::shared_ptr<hhModel> m : models) {
    m->setBuffers();
  } 
}

void hhLayer::renderAllModels() {
  std::shared_ptr<hhMaterial> currentMaterial;
  for (std::shared_ptr<hhModel>& currentModel : models) {
    if (currentModel) {
      currentModel->renderModel();
    } else {
      std::cout << "null model ptr found in renderLayer()" << std::endl;
    }
  }
}


#endif