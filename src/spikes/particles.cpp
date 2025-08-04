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

  std::shared_ptr<hhMesh3d> cloud = std::make_shared<hhMesh3d>(hhMeshFactory::makeGridAlignedParticles(glm::vec3(-4.0), glm::vec3(4.0), glm::vec3(12)));

  std::shared_ptr<hhModel3d> model1 = std::make_shared<hhModel3d>(GL_POINTS);

  model1->addMesh(cloud);
  model1->setBuffers();

  std::shared_ptr<basic3dLayer> layer1 = std::make_shared<basic3dLayer>(0);
  layer1->addModel(model1); 
  std::shared_ptr<hhCamera> cam1 = std::make_shared<hhCamera>(stage.getInputManager());
  cam1->updateSettings(glm::vec3(0.0, 0.0, 20.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0), 0.05, 0.1);
  layer1->setCamera(cam1);
  layer1->disableLighting();

  // Compile and link shaders

  hhCompShader particleShader("particleMotion/particles.comp");
  hhCompShader particleCollision("particleMotion/particleCollisions.comp");
  hhCompShader particleUpdate("particleMotion/particleUpdate.comp");

  // particle position SSBO
  std::shared_ptr<SSBO<hhVert>> pts = std::make_shared<SSBO<hhVert>>(cloud->vertices, true);
  std::vector<glm::vec4> v(12 * 12 * 12);

  std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<float> dist(-1.0, 1.0);
  for (int i = 0; i < 12 * 12 * 12; i++) {
    v[i] = glm::vec4(dist(rng), dist(rng), dist(rng), 0.0);
  }

  // velocities SSBO "v"
  std::shared_ptr<SSBO<glm::vec4>> velocity = std::make_shared<SSBO<glm::vec4>>(v, true);

  particleShader.addSSBO(pts, 0);
  particleShader.addSSBO(velocity, 1);
  particleCollision.addSSBO(pts, 0);
  particleCollision.addSSBO(velocity, 1);
  particleUpdate.addSSBO(pts, 0);
  particleUpdate.addSSBO(velocity, 1);

  // Main render loop
  glEnable(GL_DEPTH_TEST);

  std::vector<hhVert> testVec(cloud->vertices.size());
  for (int i = 0; i < cloud->vertices.size(); i++) {
    testVec[i].pos = cloud->vertices[i].pos;
  }

  glPointSize(2.5f);

  layer1->activeEffects.hBlur = 0.3;
  layer1->activeEffects.vBlur = 0.3;
  stage.addLayer(layer1);

  while (stage.doRenderLoop()) {
    
    // model1->rotate(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 1.0f, 0.0f), static_cast<float>(0.0005));

    particleShader.dispatch(glm::vec3(12, 12, 12));
    particleCollision.dispatch(glm::vec3(12, 12, 12));
    particleUpdate.dispatch(glm::vec3(12, 12, 12));

    cloud->update(pts->retrieve());

    stage.render();

  }

  glfwTerminate();
  return 0;
}

*/