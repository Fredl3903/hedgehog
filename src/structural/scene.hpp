#include <vector>
#include <memory>
#include <algorithm>
#include "../util/vecSort.hpp"

#include "layer.hpp"

class hhScene {
  
  private:
    int numLayers = 0;
    std::vector<std::shared_ptr<hhLayer>> layers;

  public:

    hhScene() {};
    // move constructor
    hhScene(hhScene&& other) noexcept
      : numLayers(other.numLayers),
        layers(std::move(other.layers)) {}
    hhScene& operator=(hhScene&& other) noexcept {
      if (this != &other) {
        numLayers = other.numLayers;
        layers = std::move(other.layers);
      }
      return *this;
    }

    void addLayer(const std::shared_ptr<hhLayer>& newLayer);
    void setAllProjMatrix(glm::mat4& projMatrix);
    void renderScene();

    void layerSort();

    int getNumLayers() const { return numLayers; }
};



// ************************************************************************************************

void hhScene::addLayer(const std::shared_ptr<hhLayer>& newLayer) {
  layers.push_back(newLayer);
  layerSort();
  numLayers += 1;
}

void hhScene::renderScene() {
  for (const std::shared_ptr<hhLayer>& layer : layers) {
    layer->renderLayer();
  }
}

void hhScene::layerSort() {
  std::sort(layers.begin(), layers.end(),
    [](const std::shared_ptr<hhLayer>& a, const std::shared_ptr<hhLayer>& b) {
      return a->order < b->order;
    });
}

void hhScene::setAllProjMatrix(glm::mat4& projMatrix) {
  for (const std::shared_ptr<hhLayer>& layer : layers) {
    layer->setProjUniforms(projMatrix);
  }
}