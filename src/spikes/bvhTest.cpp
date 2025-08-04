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
#include "hhInclude/hhCompute.hpp"
#include "hhInclude/hhLayers.hpp"

int main() {

  hhScene stage(800, 600);

  std::shared_ptr<hhMesh3d> ball = std::make_shared<hhMesh3d>(hhMeshFactory::makeIcoSphere(1, 2));
  std::shared_ptr<hhMesh3d> cow =  std::make_shared<hhMesh3d>("../src/objects/cowNorms.obj");
  std::shared_ptr<hhMesh3d> tree = std::make_shared<hhMesh3d>("../src/objects/lowPolyTree.obj");
  std::shared_ptr<hhMesh3d> room = std::make_shared<hhMesh3d>(hhMeshFactory::makeCube(24));

  ball->setCols(glm::vec3(0.8, 0.4, 0.9));
  cow->setCols(glm::vec3(0.7, 0.7, 0.95));
  tree->setCols(glm::vec3(0.3, 0.6, 0.2));

  ball->translate(glm::vec3(-1.5, 4.0, -3.0));
  cow->translate(glm::vec3(-2.5, 0.0, 3.0));
  tree->scale(glm::vec3(0.0), glm::vec3(0.4));
  tree->translate(glm::vec3(2.0, 0.0, 0.0));

  room->reverseNorms();

  std::shared_ptr<hhModel3d> model1 = std::make_shared<hhModel3d>(GL_TRIANGLES);
  model1->addMesh(ball);
  model1->addMesh(cow);
  model1->addMesh(tree);
  model1->setBuffers();

  std::shared_ptr<bvhLayer> bvhTree = std::make_shared<bvhLayer>(16, 0);
  std::shared_ptr<basic3dLayer> wireBoxes = std::make_shared<basic3dLayer>(0);
  wireBoxes->disableLighting();
  
  bvhTree->addModel(model1);
  bvhTree->build();


  std::shared_ptr<hhModel3d> bvhModel = bvhTree->visualiseBVH();
  bvhModel->setBuffers();
  wireBoxes->addModel(bvhModel);
  

  std::shared_ptr<hhFreeCam> cam1 = std::make_shared<hhFreeCam>(stage.getInputManager());
  bvhTree->setCamera(cam1);
  wireBoxes->setCamera(cam1);

  wireBoxes->activeEffects.bloom = 0.5;
  stage.addLayer(bvhTree);
  stage.addLayer(wireBoxes);

  // Main render loop
  glEnable(GL_DEPTH_TEST);

  while (stage.doRenderLoop()) {
    stage.render();
  }

  // Cleanup

  glfwTerminate();
  return 0;
}

*/