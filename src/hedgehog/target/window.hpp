#ifndef WINDOW
#define WINDOW

#include <memory>
#include <iostream>

#include <C:/OpenGL/GLEW/include/GL/glew.h>
#include <C:/OpenGL/GLFW/include/GLFW/glfw3.h>
#include <C:/OpenGL/glm/glm/glm.hpp>
#include <C:/OpenGL/glm/glm/gtc/matrix_transform.hpp>

#include "renderTarget.hpp"


class hhWindow : public hhRenderTarget {
    
  private: 
      
    float inputSensitivity;
        
  public:
    
    std::shared_ptr<InputManager> inputManager;
    std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> windowPtr;
    hhWindow(int width, int height, const char* title);

    InputManager* getInputManagerPtr() { return inputManager.get(); }
    void resizeWindow(int w, int h);

    void hedge() override;
    void hog() override;

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
  : hhRenderTarget(width, height),
  inputManager(std::make_shared<InputManager>()),
  windowPtr(nullptr, glfwWindowDeleter)
{
  std::cout << "window constructor" << std::endl;

  GLFWwindow* rawWindow = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!rawWindow) {
    std::cout << "failed to create window" << std::endl;
    return;
  }

  std::cout << "glfwCreateWindow done" << std::endl;

  windowPtr.reset(rawWindow);
  glfwMakeContextCurrent(windowPtr.get());

}

void hhWindow::destroy() {

  windowPtr.reset();
  glfwTerminate();

}

void hhWindow::hedge() {
  glViewport(0, 0, width, height);
  glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f); // Set clear color to blue
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void hhWindow::hog() {
  glfwSwapBuffers(windowPtr.get());
  glfwPollEvents();
}


#endif