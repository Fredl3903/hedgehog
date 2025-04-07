#include <C:/OpenGL/glfw/include/GLFW/glfw3.h>
#include <C:/OpenGL/glm/glm/glm.hpp>

#include "InputManager.hpp"

// camera holds view matrix and manages controls for a layer of models.


class hhCamera {

  private:

    InputManager* inputManager;

    float yaw = -90.0f;
    float pitch = 0.0f;
    float speed = 2.5f;

    glm::mat4 viewMat;

    void rotateY(float angle);
    void rotateX(float angle);
    void moveX(float dist);
    void moveY(float dist);
    void moveZ(float dist);

    virtual void processKeyboardInput();
    virtual void processMouseMove();

  public:
    glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 10.0f);

    glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    hhCamera(InputManager* inputMngr) : inputManager(inputMngr) {}
    void updateSettings(glm::vec3 pos, float speed, float sensitivity);
    void update();
    glm::mat4* getView();
};



// ************************************************************************************************

void hhCamera::updateSettings(glm::vec3 pos, float speed, float sensitivity) {
  camPos = pos;
  this->speed = speed;
  inputManager->sensitivity = sensitivity;
}

void hhCamera::moveX(float dist) {
  std::cout << "moveX - " << dist << std::endl;
  camPos += dist * right;
}
void hhCamera::moveY(float dist) {
  std::cout << "moveX - " << dist << std::endl;
  camPos += dist * up;
}
void hhCamera::moveZ(float dist) {
  std::cout << "moveX - " << dist << std::endl;
  camPos += dist * forward;
}
void hhCamera::rotateX(float angle) {
  if (abs(pitch) < 90.0f - abs(angle)) {
    rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), right);
    
    forward = glm::mat3(rotationMatrix) * forward;
    up = glm::mat3(rotationMatrix) * up;
        
    right = glm::normalize(glm::cross(forward, up));

    pitch += angle;
  }
}

void hhCamera::rotateY(float angle) {
  rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    
  forward = glm::mat3(rotationMatrix) * forward;
  up = glm::mat3(rotationMatrix) * up;
    
  right = glm::normalize(glm::cross(forward, up));
}

void hhCamera::processKeyboardInput() {
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

void hhCamera::processMouseMove() {
  if (inputManager->isMouseButtonPressed(0)) {
    std::cout << "rotating camera" << std::endl;
    
    // Calculate the delta from the center of the screen
    float deltaX, deltaY;
    inputManager->getMouseDelta(deltaX, deltaY);

    // Update the yaw and pitch based on the delta
    rotateY(deltaX);
    rotateX(deltaY);
  }
}


void hhCamera::update() {
  processKeyboardInput();
  processMouseMove();
}

glm::mat4* hhCamera::getView() {
    viewMat = glm::lookAt(camPos, camPos + 5.0f * glm::normalize(forward), up);
    return &viewMat;
}