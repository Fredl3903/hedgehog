/*


#include <C:/OpenGL/GLEW/include/GL/glew.h>
#include <C:/OpenGL/GLFW/include/GLFW/glfw3.h>
#include <C:/OpenGL/glm/glm/glm.hpp>
#include <C:/OpenGL/glm/glm/gtc/matrix_transform.hpp>
#include <C:/OpenGL/glm/glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>

#include "hhInclude/hhBasic.hpp"
#include "hhInclude/hhMeshBuild.hpp"
#include "hhInclude/hhCam.hpp"
#include "hhInclude/hhFBO.hpp"
#include "hhInclude/hhLayers.hpp"

int main() {

  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "GLFW initialization failed!" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // Create window
  hhWindow window1 = hhWindow(800, 600, "hedgehog.");
  glfwSetFramebufferSizeCallback(window1.windowPtr.get(), [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });

  // Initialize GLEW
  if (glewInit() != GLEW_OK) { 
    std::cerr << "GLEW initialization failed!" << std::endl;
    return -1;
  }

  glEnable(GL_DEBUG_OUTPUT);


  // GEOMETRY SETUP
  std::shared_ptr<hhMesh3d> icos = std::make_shared<hhMesh3d>("../src/objects/shed.obj");
  
  std::shared_ptr<hhModel3d> icosModel = std::make_shared<hhModel3d>(GL_TRIANGLES);
  icosModel->addMesh(icos);

  icosModel->setBuffers();

  std::shared_ptr<screenQuadLayer> layer1 = std::make_shared<screenQuadLayer>();

  std::shared_ptr<blinnPhongLayer> layer2 = std::make_shared<blinnPhongLayer>(0);

  layer2->addModel(icosModel);

  std::shared_ptr<hhFreeCam> cam1 = std::make_shared<hhFreeCam>(window1.getInputManagerPtr());
  layer2->setCamera(cam1);

  std::shared_ptr<hhScene> scene1 = std::make_shared<hhScene>();
  scene1->addLayer(layer1);
  std::shared_ptr<hhScene> scene2 = std::make_shared<hhScene>();
  scene2->addLayer(layer2);

  std::shared_ptr<hhFBO> fbo = std::make_shared<hhFBO>(800, 600);
  fbo->addScene(scene2);

  layer1->setImage(fbo->colorTex);

  window1.addScene(scene1);

  // Main render loop
  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window1.windowPtr.get())) {

    fbo->renderLoop(0);

    window1.renderLoop(0);

  }

  glfwTerminate();
  return 0;
}


*/