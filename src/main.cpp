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
#include "hhInclude/hhCompute.hpp"
#include "hhInclude/hhLayers.hpp"

int main() {

  hhScene stage(800, 600);

  std::shared_ptr<basic3dLayer> bvhTree = std::make_shared<basic3dLayer>(0);
  std::shared_ptr<hhMesh3d> cow = std::make_shared<hhMesh3d>("../src/objects/cowNorms.obj");
  std::shared_ptr<hhModel3d> cowModel = std::make_shared<hhModel3d>(GL_TRIANGLES);
  cowModel->addMesh(cow);
  cowModel->setBuffers();
  bvhTree->addModel(cowModel);
  
  std::shared_ptr<hhFreeCam> cam1 = std::make_shared<hhFreeCam>(stage.getInputManager());
  cam1->updateSettings(glm::vec3(0.0, 0.0, 15.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0), 0.1, 0.1);
  bvhTree->setCamera(cam1);

  stage.addLayer(bvhTree);

  // Main render loop
  glEnable(GL_DEPTH_TEST);

  while (stage.doRenderLoop()) {
    stage.render();
  }

  // Cleanup

  glfwTerminate();
  return 0;
}