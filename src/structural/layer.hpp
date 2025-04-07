#include <memory>

#include <C:/OpenGL/GLEW/include/GL/glew.h>
#include <C:/OpenGL/glm/glm/gtc/type_ptr.hpp>

#include "model.hpp"
#include "camera.hpp"
#include "../glObjects/shaderProgram.hpp"


class hhLayer {

  private:
        
    int numModels = 0;
    std::vector<std::weak_ptr<hhModel>> models;
    std::weak_ptr<hhCamera> layerCamera;
    std::weak_ptr<hhShaderProgram> layerShaders;

    // add in JSON member that describes the structure of layer uniforms
    // addLayerUniform() method 

  public:

    unsigned int order = 0;
        
    hhLayer(int zIndex) : order(zIndex) {}
    // move constructor
    hhLayer(hhLayer&& other) noexcept
      : numModels(other.numModels),
        models(std::move(other.models)),
        layerCamera(std::move(other.layerCamera)),
        layerShaders(std::move(other.layerShaders)),
        order(other.order) {}

    void addModel(const std::shared_ptr<hhModel>& newModel);

    void setLayerShaders(const std::shared_ptr<hhShaderProgram>& newShaderProgram) {
      layerShaders = newShaderProgram;
    };
    void setCamera(const std::shared_ptr<hhCamera>& newCamera) {
      layerCamera = newCamera;
    };
    // when rendering a layer, set view and projection matrices first from camera then loop over the models
    void setCamUniforms(); // view matrix from camera
    void setModelUniforms(std::shared_ptr<hhModel>& model); // model matrix from each model
    void setProjUniforms(glm::mat4& projMat); // projection matrix from window via scene
    template<typename T>
    void setLayerUniform(std::string name, T uVal);
    void renderLayer();
        
    unsigned int getZIndex() const { return order; }    
    void destroy() {};
    
    ~hhLayer() {
      destroy();
    }
};




// ************************************************************************************************


void hhLayer::addModel(const std::shared_ptr<hhModel>& newModel) {        
  models.push_back(newModel);
  numModels += 1;
}

void hhLayer::setCamUniforms() {// view matrix (model matrix set per model)
  std::shared_ptr<hhCamera> sharedCamera = layerCamera.lock();
  std::shared_ptr<hhShaderProgram> sharedShader = layerShaders.lock();
  
  if (!sharedCamera || !sharedShader) {
    std::cout << "null weak_ptr in setCamUniforms" << std::endl;
    return;
  }
  
  int uViewMatLocation = glGetUniformLocation(sharedShader->getID(), "viewMat");
  glm::mat4* uViewMat = sharedCamera->getView();

  glUniformMatrix4fv(uViewMatLocation, 1, GL_FALSE, glm::value_ptr(*uViewMat));
}

void hhLayer::setModelUniforms(std::shared_ptr<hhModel>& model) {
  if (!model) {
    std::cout << "null shared_ptr to model specified in setModelUniforms" << std::endl;
    return;
  }

  std::shared_ptr<hhShaderProgram> sharedShader = layerShaders.lock();
  if (!sharedShader) {
    std::cout << "null weak_ptr to shaderProgram in setModelUniforms" << std::endl;
    return;
  }

  int uModelMatLocation = glGetUniformLocation(sharedShader->getID(), "modelMat");
  glm::mat4* uModelMat = &model->modelMatrix;

  glUniformMatrix4fv(uModelMatLocation, 1, GL_FALSE, glm::value_ptr(*uModelMat));
}

void hhLayer::setProjUniforms(glm::mat4& projMat) {
  
  std::shared_ptr<hhShaderProgram> sharedShader = layerShaders.lock();
  if (!sharedShader) {
    std::cout << "null weak_ptr to shaderProgram in setProjUniforms" << std::endl;
    return;
  }

  int uProjMatLocation = glGetUniformLocation(sharedShader->getID(), "projMat");
  glm::mat4* uProjMat = &projMat;

  glUniformMatrix4fv(uProjMatLocation, 1, GL_FALSE, glm::value_ptr(*uProjMat));
}

template<typename T>
void hhLayer::setLayerUniform(std::string name, T uVal) {

  if (layerShaders.lock()) {
    int uniformLoc = glGetUniformLocation(layerShaders.lock()->getID(), name.c_str());
    if (uniformLoc == -1) {
      std::cout << "Could not find uniform " << name << std::endl;
      return;
    }

    if constexpr (std::is_same<T, int>::value) {
      glUniform1i(uniformLoc, uVal);
    } else if constexpr (std::is_same<T, float>::value) {
      glUniform1f(uniformLoc, uVal);
    } else if constexpr (std::is_same<T, glm::vec2>::value) {
      glUniform2fv(uniformLoc, 1, glm::value_ptr(uVal));
    } else if constexpr (std::is_same<T, glm::vec3>::value) {
      glUniform3fv(uniformLoc, 1, glm::value_ptr(uVal));
    } else if constexpr (std::is_same<T, glm::vec4>::value) {
      glUniform4fv(uniformLoc, 1, glm::value_ptr(uVal));
    } else if constexpr (std::is_same<T, glm::mat2>::value) {
      glUniformMatrix2fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(uVal));
    } else if constexpr (std::is_same<T, glm::mat3>::value) {
      glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(uVal));
    } else if constexpr (std::is_same<T, glm::mat4>::value) {
      glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(uVal));
    } else {
      std::cout << "Unsupported type for uniform " << name << std::endl;
    }
  } else {
    std::cout << "Shader program is not set" << std::endl;
  }
}


void hhLayer::renderLayer() {

  // Get shared pointers from weak pointers
  std::shared_ptr<hhCamera> sharedCam = layerCamera.lock();
  std::shared_ptr<hhShaderProgram> sharedShader = layerShaders.lock();
  
  // Check both pointers are valid before proceeding
  if (!sharedCam) {
    std::cout << sharedCam << " - null weak_ptr<hhCamera> in renderLayer()" << std::endl;
    return; // Exit early if camera is null
  }
  
  if (!sharedShader) {
    std::cout << "null weak_ptr<hhShaderProgram> in renderLayer()" << std::endl;
    return; // Exit early if shader is null
  }

  sharedShader->use();
  sharedCam->update();
  setCamUniforms();
  // setLayerUniforms();

  for (std::weak_ptr<hhModel>& currentModel : models) {
    std::shared_ptr<hhModel> sharedModel = currentModel.lock();
    if (sharedModel) {
      setModelUniforms(sharedModel); // model matrix
      sharedModel->renderModel();
    } else {
      std::cout << "null weak_ptr<hhModel> found in renderLayer()" << std::endl;
    }
  }
};