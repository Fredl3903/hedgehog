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

  // MATERIAL SETUP
  const int texWidth = 250;
  const int texHeight = 188;

  std::shared_ptr<hhImgTex> testCol = std::make_shared<hhImgTex>(texWidth, texHeight);
  std::shared_ptr<hhImgTex> testImg = std::make_shared<hhImgTex>("scratch.jpg");

  std::shared_ptr<hhMaterial> testMaterialCol = std::make_shared<hhMaterial>();
  testMaterialCol->setTexture1(testCol);
  testMaterialCol->setEmission(glm::vec3(1.0, 1.0, 1.0));
  testMaterialCol->setSurface(0.1, 0.9);
  std::shared_ptr<hhMaterial> testMaterialImg = std::make_shared<hhMaterial>();
  testMaterialImg->setTexture1(testImg);
  testMaterialImg->setEmission(glm::vec3(0.8, 0.0, 0.6));
  testMaterialImg->setSurface(0.8, 0.3);
  
  // GEOMETRY SETUP
  std::shared_ptr<hhMesh3d> cube = std::make_shared<hhMesh3d>(hhMeshFactory::makeCube(5.0));
  std::shared_ptr<hhMesh3d> sphere = std::make_shared<hhMesh3d>(hhMeshFactory::makeIcoSphere(2.0, 3));
  sphere->setCols(glm::vec3(1.0));

  std::shared_ptr<hhModel3d> model1 = std::make_shared<hhModel3d>(GL_TRIANGLES);
  model1->addMesh(sphere);
  model1->setBuffers();
  model1->addMaterial(testMaterialCol);
  model1->translate(glm::vec3(-6.0, 3.0, 0.0));

  std::shared_ptr<hhModel3d> model2 = std::make_shared<hhModel3d>(GL_TRIANGLES);
  model2->addMesh(cube);
  model2->setBuffers();
  model2->addMaterial(testMaterialImg);
  model2->translate(glm::vec3(6.0, 3.0, 0.0));

  // SCENE STRUCTURE
  std::shared_ptr<blinnPhongLayer> layer1 = std::make_shared<blinnPhongLayer>(0);
  layer1->addModel(model1);
  layer1->addModel(model2);
  std::shared_ptr<hhFreeCam> cam1 = std::make_shared<hhFreeCam>(stage.getInputManager());
  cam1->updateSettings(glm::vec3(0.0, 0.0, 20.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0), 0.1, 0.1);
  layer1->setCamera(cam1);

  layer1->activeEffects.bloom = 1.0;

  stage.addLayer(layer1);

  // Main render loop
  glEnable(GL_DEPTH_TEST);
  glPointSize(2.5f);
  
  while (stage.doRenderLoop()) {
    stage.render();
  }
  glfwTerminate();
  return 0;
}


*/