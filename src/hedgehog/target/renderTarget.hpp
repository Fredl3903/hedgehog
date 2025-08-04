#ifndef RENDER_TARGET
#define RENDER_TARGET

#include <memory>
#include <iostream>

#include <C:/OpenGL/GLEW/include/GL/glew.h>
#include <C:/OpenGL/GLFW/include/GLFW/glfw3.h>
#include <C:/OpenGL/glm/glm/glm.hpp>
#include <C:/OpenGL/glm/glm/gtc/matrix_transform.hpp>

#include "../cam/InputManager.hpp"
#include "../layer/layer.hpp"

class hhRenderTarget {
        
  public:
    
    glm::vec4 clearColor = glm::vec4(0.8f, 0.78f, 0.8f, 0.0f);
    
    int width;
    int height;
    float fov = glm::radians(45.0f);
    float aspect = width / height;
    float near = 0.1f;
    float far = 300.0f;
    
    glm::mat4 projectionMatrix = glm::perspective(fov, aspect, near, far);
    glm::mat4 orthoProjMatrix = glm::ortho(0.0f, float(width), 0.0f, float(height));
    
    hhRenderTarget(int width = 800, int height = 600) 
        : width(width), height(height), aspect(float(width) / float(height)) {
        projectionMatrix = glm::perspective(fov, aspect, near, far);
    }

    virtual void hedge() = 0; // prepare render loop
    virtual void hog() = 0; // end render loop 
    // void debugOutput();

    void render(std::shared_ptr<hhLayer> layer) {
      if (!layer) {
        std::cout << "invalid layer ptr provided" << std::endl;
        return;
      }

      if (layer->dimensionality == 2) {
        layer->setProjMatrix(orthoProjMatrix);  
      } else {
        layer->setProjMatrix(projectionMatrix);
      }
      layer->renderLayer();
    }

    virtual ~hhRenderTarget() = default;

};


// void hhRenderTarget::debugOutput() {
//   // currently checks 0th model from 0th layer of 0th scene
//   // adapt at some point to check all

//   std::shared_ptr<hhLayer> checkLayer = scenes[0]->layers[0];
//     std::shared_ptr<hhModel> checkModel;
//     if (!checkLayer) {
//       std::cout << "failed to find layers[0]" << std::endl;
//     }
//     std::shared_ptr<hhShaderProgram> checkShaders = (checkLayer->layerShaders);
//     if (!checkShaders) {
//       std::cout << "shaders not found" << std::endl;
//     } else {
//       std::cout << "shaderProgramID: " << checkShaders->getID() << std::endl;
//     }
//     std::shared_ptr<hhCamera> checkCam = (checkLayer->layerCamera);
//     if (!checkCam) {
//       std::cout << "camera not found" << std::endl;
//     } else {
//       glm::vec3 camPos = checkCam->camPos;
//       glm::vec3 camAt = checkCam->forward;
//       std::cout << "camera position: ( " << camPos.x << ", "
//         << camPos.y << ", "
//         << camPos.z << " )" << std::endl;
//       std::cout << "camera forward: ( " << camAt.x << ", "
//         << camAt.y << ", "
//         << camAt.z << " )" << std::endl;
//     }
//     checkModel = (checkLayer->models[0]);
//     if (!checkModel) {
//       std::cout << "failed to find models[0]" << std::endl;
//     } else {
//       checkModel->debugOutput();
//     }
// }


#endif