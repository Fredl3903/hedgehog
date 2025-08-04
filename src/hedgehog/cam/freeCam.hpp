#ifndef FREE_CAM
#define FREE_CAM

#include <iostream>
#include "camera.hpp"

class hhFreeCam : public hhCamera {

  public:

    hhFreeCam(InputManager* inputMngr) : hhCamera(inputMngr) {}

    void processKeyboardInput() override;
    void processMouseMove() override;

};

void hhFreeCam::processKeyboardInput() {
  if (inputManager == nullptr) {
    std::cout << "input manager not found" << std::endl;
    return;
  }
  if (inputManager->isKeyPressed(GLFW_KEY_W)) {
    moveZ(speed);
  } else if (inputManager->isKeyPressed(GLFW_KEY_A)) {
    moveX(-speed);   
  } else if (inputManager->isKeyPressed(GLFW_KEY_S)) {
    moveZ(-speed);   
  } else if (inputManager->isKeyPressed(GLFW_KEY_D)) {
    moveX(speed);   
  } else if (inputManager->isKeyPressed(GLFW_KEY_SPACE)) {
    moveY(speed);
  } else if (inputManager->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
    moveY(-speed);   
  }
}

void hhFreeCam::processMouseMove() {
  if (inputManager->isMouseButtonPressed(0)) {    
    // Calculate the delta from the center of the screen
    float deltaX, deltaY;
    inputManager->getMouseDelta(deltaX, deltaY);

    // Update the yaw and pitch based on the delta
    rotateY(deltaX);
    rotateX(deltaY);
  }
}

#endif