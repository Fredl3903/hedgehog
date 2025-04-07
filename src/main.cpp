#include <C:/OpenGL/GLEW/include/GL/glew.h>
#include <C:/OpenGL/GLFW/include/GLFW/glfw3.h>
#include <C:/OpenGL/glm/glm/glm.hpp>
#include <iostream>

#include "structural/window.hpp"

std::unordered_map<std::string, attribData> simpleMeshGenerator(float A, float B, float C) {  
  std::unordered_map<std::string, attribData> attributes;

  // Position attribute
  auto positions = std::make_shared<std::vector<float>>(std::vector<float>{
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
  });

  attributes["pos"] = attribData{
    3,        // Each vertex has 3 elements (x, y, z)
    positions,
    nullptr   // No indices for this attribute
  };

  // Color attribute
  auto colors = std::make_shared<std::vector<float>>(std::vector<float>{
    0.8, 0.5, 0.9,
    0.8, 0.5, 0.9,
    0.8, 0.5, 0.9,
    0.8, 0.5, 0.9,
    0.8, 0.5, 0.9,
    0.8, 0.5, 0.9,
    0.8, 0.5, 0.9,
    0.8, 0.5, 0.9,
  });

  attributes["col"] = attribData{
    3,        // Each vertex has 3 elements (x, y, z)
    colors,
    nullptr   // No indices for this attribute
  };

  // Indices attribute (stored separately)
  auto indices = std::make_shared<std::vector<unsigned int>>(std::vector<unsigned int>{
    0, 1, 2, 2, 3, 0,
    0, 4, 5, 5, 1, 0,
    0, 3, 7, 7, 4, 0,
    3, 2, 6, 6, 7, 3,
    2, 1, 5, 5, 6, 2,
    4, 7, 6, 6, 5, 4
  });

  attributes["indices"] = attribData{
    1,         // Element length is 1 for index data
    nullptr,   // No vertex data here
    indices
  };

  return attributes;
}

const int width = 800;
const int height = 600;

int main() {
    

  // ************ BASIC SETUP ************

  if (!glfwInit()) {
    std::cerr << "failed to initialise GLFW" << std::endl;
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  hhWindow window1(800, 600, "hedgehog.");

  std::cout << glGetString(GL_VERSION) << std::endl;
  GLenum glewInitResult = glewInit();
  if (glewInitResult != GLEW_OK) {
    std::cerr << "failed to initialise GLEW: " << glewGetErrorString(glewInitResult) << std::endl;
    window1.destroy();
    return -1;
  } else {
    std::cout << "success" << std::endl;
  }

  // ************ INITIAL VERTEX DATA ************


  // ************ SHADER SETUP ************

  std::shared_ptr<hhShaderProgram> basicShaderProgram = std::make_shared<hhShaderProgram>("blank3D.vert","blank3D.frag");

  // ************ SCENE SETUP ************

  hhScene scene1 = hhScene();
  std::shared_ptr<hhLayer> layer1 = std::make_shared<hhLayer>(0);

  std::shared_ptr<hhCamera> cam1 = std::make_shared<hhCamera>(window1.getInputManagerPtr());
  cam1->updateSettings(glm::vec3(0.0, 0.0, 50), 1.0, 0.1);
  layer1->setCamera(cam1);

  // can also just do layer1.addModel(hhModel(GL_TRIANGLES)) then call addMesh like layer1.models[i]->addMesh();
  std::shared_ptr<hhModel> model1 = std::make_shared<hhModel>(GL_TRIANGLES);
  hhMesh mesh1 = hhMesh(3, simpleMeshGenerator, 1.0f, 1.0f, 1.0f);



  // ****************************************************************

  model1->addMesh(mesh1);
  layer1->addModel(model1);
  layer1->setLayerShaders(basicShaderProgram);
  scene1.addLayer(layer1);
  window1.addScene(std::move(scene1));

  // ************ RENDER LOOP ************  
  std::cout << "****************************** RENDER LOOP BEGIN ******************************" << std::endl;
  std::cout << "*******************************************************************************" << std::endl;
  while(!glfwWindowShouldClose(window1.windowPtr.get())) {
    // update models
    window1.renderLoop(0);
  }


  window1.destroy();
  glfwTerminate();

  return 0;
}
