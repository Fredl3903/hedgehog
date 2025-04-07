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
    std::vector<std::unique_ptr<hhScene>> scenes;
    
    std::shared_ptr<InputManager> inputManager;
        
  public:

    int windowWidth = 800;
    int windowHeight = 600;
    std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> windowPtr;

    hhWindow(int width, int height, const char* title);

    InputManager* getInputManagerPtr() { return inputManager.get(); }
    void resizeWindow(int w, int h);
    void addScene(hhScene&& newScene);

    void renderLoop(int scene);

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
  scenes[currentScene]->renderScene();

  glfwSwapBuffers(windowPtr.get());
  glfwPollEvents();
}