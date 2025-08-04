#ifndef LIGHT
#define LIGHT

#include <C:/OpenGL/glm/glm/glm.hpp>

struct gpuLight {
  glm::vec4 pos = glm::vec4(0.0, 5.0, 0.0, 0.0);
  glm::vec4 dir = glm::vec4(0.0, -1.0, 0.0, 0.0);
  glm::vec4 col = glm::vec4(1.0, 1.0, 1.0, 0.0);
  
  glm::vec4 params = glm::vec4(0.5, 0.0, 1.0, 0.0);
  // intensity, type, spotlightAngle

  gpuLight(
    glm::vec3 position, glm::vec3 direction, glm::vec3 color,
    float intensity, int lightType, float angle
  ) : pos(glm::vec4(position, 0.0)), dir(glm::vec4(direction, 0.0)), col(glm::vec4(color, 1.0)),
    params(glm::vec4(intensity, lightType, angle, 0.0)) {}
};

class hhLight {

  public:

    glm::vec3 pos = glm::vec3(0.0, 5.0, 0.0);
    glm::vec3 dir = glm::vec3(0.0, -1.0, 0.0);
    glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);
    float intensity = 0.5;

    unsigned int lightType = 0;
    // 0 - point source; 1 - sun; 2 - spotlight
    float spotlightAngle = 1.0;

    hhLight() = default;
    hhLight(glm::vec3 col) : color(col) {}
    
    void setCol(glm::vec3 col) { color = col; }
    void setIntensity(float intensity) { this->intensity = intensity; }

    void point() { lightType = 0; }
    void sun() { lightType = 1; }
    void spotlight(float angle) { 
      lightType = 2;
      spotlightAngle = angle;
    }

    gpuLight getCompact() {
      return gpuLight(
        pos, dir, color, intensity, lightType, spotlightAngle
      );
    }

};

#endif