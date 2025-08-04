// inputManager.hpp
#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#include <C:/OpenGL/GLFW/include/GLFW/glfw3.h>
#include <unordered_map>

class InputManager {
private:
    // Keep track of key states (pressed or not)
    std::unordered_map<int, bool> keyStates;

    // Mouse button states
    std::unordered_map<int, bool> mouseButtonStates;

    // Mouse position (current and delta)
    double mouseX, mouseY;
    double prevMouseX, prevMouseY;

public:

    // Sensitivity for mouse movement
    float sensitivity;

    InputManager();
    
    // Updates the state of the keys and mouse
    void update(GLFWwindow* window);

    // Query key states
    bool isKeyPressed(int key);
    
    // Query mouse button states
    bool isMouseButtonPressed(int button);

    // Get the mouse delta (movement since last frame)
    void getMouseDelta(float& deltaX, float& deltaY);

    // Set the mouse position (if needed, e.g., for centering the mouse)
    void setMousePosition(GLFWwindow* window, double x, double y);
};

// Constructor to initialize sensitivity
InputManager::InputManager() : sensitivity(0.1), mouseX(0.0), mouseY(0.0), prevMouseX(0.0), prevMouseY(0.0) {
  std::cout << "test3" << std::endl;
}

// Update key and mouse button states, as well as mouse delta
void InputManager::update(GLFWwindow* window) {
  // Update key states (pressed or not)
  for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
    keyStates[key] = glfwGetKey(window, key) == GLFW_PRESS;
  }
  // Update mouse button states
  for (int button = GLFW_MOUSE_BUTTON_1; button <= GLFW_MOUSE_BUTTON_LAST; ++button) {
    mouseButtonStates[button] = glfwGetMouseButton(window, button) == GLFW_PRESS;
  }
  // Update mouse position and delta
  prevMouseX = mouseX;
  prevMouseY = mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);
}

// Check if a specific key is currently pressed
bool InputManager::isKeyPressed(int key) {
  return keyStates[key];
}

// Check if a specific mouse button is currently pressed
bool InputManager::isMouseButtonPressed(int button) {
  return mouseButtonStates[button];
}

// Get the mouse delta (movement since the last frame)
void InputManager::getMouseDelta(float& deltaX, float& deltaY) {
  deltaX = (float)((mouseX - prevMouseX) * sensitivity);
  deltaY = (float)((mouseY - prevMouseY) * sensitivity);
}

// Set the mouse position (e.g., to center the cursor for FPS-like controls)
void InputManager::setMousePosition(GLFWwindow* window, double x, double y) {
  glfwSetCursorPos(window, x, y);
}

#endif
