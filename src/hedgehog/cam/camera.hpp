#ifndef CAMERA
#define CAMERA

#include <C:/OpenGL/glfw/include/GLFW/glfw3.h>
#include <C:/OpenGL/glm/glm/glm.hpp>
#include <C:/OpenGL/glm/glm/gtc/matrix_transform.hpp>

#include "InputManager.hpp"

// camera holds view matrix and manages controls for a layer of models.


class hhCamera {

  public:

    float yaw = -90.0f;
    float pitch = 0.0f;
    float speed = 0.02f;

    glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    InputManager* inputManager;

    glm::mat4 viewMat;

    void rotateY(float angle);
    void rotateX(float angle);
    void moveX(float dist);
    void moveY(float dist);
    void moveZ(float dist);

    virtual void processKeyboardInput() {};
    virtual void processMouseMove() {};

    glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 10.0f);

    hhCamera(InputManager* inputMngr) : inputManager(inputMngr) {}
    void updateSettings(glm::vec3 pos, glm::vec3 look, glm::vec3 up, float speed, float sensitivity);
    void update();
    glm::mat4* getView();
};


void hhCamera::updateSettings(glm::vec3 pos, glm::vec3 look, glm::vec3 up, float speed, float sensitivity) {
  camPos = pos;
  forward = glm::normalize(look);
  up = glm::normalize(up);
  right = glm::normalize(glm::cross(forward, up));
  this->speed = speed;
  inputManager->sensitivity = sensitivity;
}

void hhCamera::moveX(float dist) {
  camPos += dist * right;
}
void hhCamera::moveY(float dist) {
  camPos += dist * up;
}
void hhCamera::moveZ(float dist) {
  camPos += dist * forward;
}
void hhCamera::rotateX(float angle) {
  if (abs(pitch + angle) <= 90.0f) {

    pitch += angle;

    rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-angle), right);
    
    forward = glm::mat3(rotationMatrix) * forward;
    up = glm::mat3(rotationMatrix) * up;
        
    right = glm::normalize(glm::cross(forward, up));

  }
}

void hhCamera::rotateY(float angle) {
  rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
    
  forward = glm::mat3(rotationMatrix) * forward;
  up = glm::mat3(rotationMatrix) * up;
    
  right = glm::normalize(glm::cross(forward, up));
}


void hhCamera::update() {
  processKeyboardInput();
  processMouseMove();
}

glm::mat4* hhCamera::getView() {
    viewMat = glm::lookAt(camPos, camPos + 5.0f * glm::normalize(forward), up);
    return &viewMat;
}

#endif