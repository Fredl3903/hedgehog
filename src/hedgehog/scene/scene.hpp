#ifndef SCENE
#define SCENE

#include <vector>
#include <memory>
#include <algorithm>
#include "../util/vecSort.hpp"
#include "../layer/layer.hpp"
#include "../target/window.hpp"
#include "../target/fbo.hpp"
#include "glManager.hpp"
#include "shaderManager.hpp"

class hhScene {
  
  private:
    
    int numLayers = 0;

    hhManager uniformManager;
    hhShaderManager shaderManager;

  public:
    
    int width;
    int height;
    
    hhScene(int w, int h);

    // targets
    std::unique_ptr<hhWindow> window;
    
    std::unique_ptr<hhFBO> fboA;
    std::unique_ptr<hhFBO> fboB;
    bool swapped = false;

    // layers
    std::vector<std::shared_ptr<hhLayer>> layers;
    std::shared_ptr<screenQuadLayer> outLayer;
    
    // constructors
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

    void init();

    void addLayer(const std::shared_ptr<hhLayer>& newLayer);
    void layerSort();

    int getNumLayers() const { return numLayers; }

    InputManager* getInputManager() { return window->getInputManagerPtr(); }

    void render();
    bool doRenderLoop() { return !glfwWindowShouldClose(window->windowPtr.get()); }

};

void hhScene::init() {

  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "GLFW initialization failed!" << std::endl;
    return;
  }

  window = std::make_unique<hhWindow>(width, height, "hedgehog.");

  std::cout << "window constructed" << std::endl;

  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // Create window
  glfwSetFramebufferSizeCallback(window->windowPtr.get(), [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });

  // Initialize GLEW
  if (glewInit() != GLEW_OK) { 
    std::cerr << "GLEW initialization failed!" << std::endl;
    return;
  }

  glEnable(GL_DEBUG_OUTPUT);

}



// ************************************************************************************************

void hhScene::addLayer(const std::shared_ptr<hhLayer>& newLayer) {
  layers.push_back(newLayer);
  layerSort();
  numLayers += 1;

  shaderManager.compareLayer(newLayer);
}

void hhScene::layerSort() {
  std::sort(layers.begin(), layers.end(),
    [](const std::shared_ptr<hhLayer>& a, const std::shared_ptr<hhLayer>& b) {
      return a->order < b->order;
    });
}

#endif