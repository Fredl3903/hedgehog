// need definitions:
//      resizeWindow()
//      framebufferSizeCallback

#include <memory>
#include <iostream>

#include <C:/OpenGL/GLEW/include/GL/glew.h>
#include <C:/OpenGL/GLFW/include/GLFW/glfw3.h>
#include <C:/OpenGL/glm/glm/glm.hpp>
#include <C:/OpenGL/glm/glm/gtc/matrix_transform.hpp>

#include "scene.hpp"
#include "InputManager.hpp"


class hhWindow {
    
  private: 
        
    float inputSensitivity;
    float fov = glm::radians(45.0f);
    float aspect = windowWidth / windowHeight;
    float near = 0.1f;
    float far = 1000.0f;
    glm::vec4 clearColor = glm::vec4(0.1f, 0.0f, 0.1f, 0.0f);

    int numScenes = 0;
    int currentScene;
    
    std::shared_ptr<InputManager> inputManager;
        
  public:
    
    int windowWidth = 800;
    int windowHeight = 600;
    std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> windowPtr;

    std::vector<std::unique_ptr<hhScene>> scenes;
    
    hhWindow(int width, int height, const char* title);

    InputManager* getInputManagerPtr() { return inputManager.get(); }
    void resizeWindow(int w, int h);
    void addScene(hhScene&& newScene);

    void renderLoop(int scene);
    void debugOutput();

    void updateInput() {
      inputManager->update(windowPtr.get());
    }

    void destroy();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    
    ~hhWindow() {
      destroy();
    }

};




// ************************************************************************************************


void glfwWindowDeleter(GLFWwindow* window) {
  if (window) {
    std::cout << "Destroying GLFW window.\n";
    glfwDestroyWindow(window);
  }
}

hhWindow::hhWindow(int width, int height, const char* title)
  : windowPtr(nullptr, glfwWindowDeleter),
  inputManager(std::make_shared<InputManager>())
{

  GLFWwindow* rawWindow = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!rawWindow) {
    std::cout << "failed to create window" << std::endl;
    return;
  }

  windowPtr.reset(rawWindow);
  glfwMakeContextCurrent(windowPtr.get());

}

void hhWindow::destroy() {

  windowPtr.reset();
  glfwTerminate();

}




void hhWindow::addScene(hhScene&& newScene) {
  scenes.push_back(std::make_unique<hhScene>(std::move(newScene)));
  numScenes += 1;
}

void hhWindow::renderLoop(int scene) {
  // while (!glfwWindowShouldClose(windowPtr.get())) {

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set clear color to blue
  glClear(GL_COLOR_BUFFER_BIT);
  updateInput();
  glm::mat4 projectionMatrix = glm::perspective(fov, aspect, near, far);

  if (scenes[scene]) {
    scenes[scene]->setAllProjMatrix(projectionMatrix);
  } else {
    std::cout << "not found" << std::endl;   
  }

  // std::cout << "rendering now" << std::endl;
  // rendering stuff goes here
  int currentScene = 0;
  debugOutput();
  scenes[currentScene]->renderScene();

  glfwSwapBuffers(windowPtr.get());
  glfwPollEvents();
}






void hhWindow::debugOutput() {
  // currently checks 0th model from 0th layer of 0th scene
  // adapt at some point to check all

  std::shared_ptr<hhLayer> checkLayer = scenes[0]->layers[0];
    std::shared_ptr<hhModel> checkModel;
    if (!checkLayer) {
      std::cout << "failed to find layers[0]" << std::endl;
    }
    std::shared_ptr<hhShaderProgram> checkShaders = (checkLayer->layerShaders).lock();
    if (!checkShaders) {
      std::cout << "shaders not found" << std::endl;
    } else {
      std::cout << "shaderProgramID: " << checkShaders->getID() << std::endl;
    }
    std::shared_ptr<hhCamera> checkCam = (checkLayer->layerCamera).lock();
    if (!checkCam) {
      std::cout << "camera not found" << std::endl;
    } else {
      glm::vec3 camPos = checkCam->camPos;
      glm::vec3 camAt = checkCam->forward;
      std::cout << "camera position: ( " << camPos.x << ", "
        << camPos.y << ", "
        << camPos.z << " )" << std::endl;
      std::cout << "camera forward: ( " << camAt.x << ", "
        << camAt.y << ", "
        << camAt.z << " )" << std::endl;
    }
    checkModel = (checkLayer->models[0]).lock();
    if (!checkModel) {
      std::cout << "failed to find models[0]" << std::endl;
    } else {
      checkModel->debugOutput();
    }
}