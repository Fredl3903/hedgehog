#ifndef MODEL
#define MODEL

#include <vector>
#include <memory>

#include <C:/OpenGL/glm/glm/glm.hpp>
#include <C:/OpenGL/GLEW/include/GL/glew.h>
#include <C:/OpenGL/glm/glm/gtc/matrix_transform.hpp>

#include "mesh.hpp"
#include "../material/material.hpp"
#include "../buffer/VAO.hpp"


int findPosIndex(int nextIndex, const std::vector<unsigned int>& tempIndices) {
    for (int i = 0; i < tempIndices.size(); i++) {
      if (tempIndices[i] == nextIndex) {
        return i;
      }
    }
    return -1;
  };


class hhModel {

  public:
    
    GLenum renderMode; // same for all meshes

    VAO modelVAO;
    VBO modelVBO;
    IBO modelIBO;

    bool bufferUpdateNeeded = false;

    bool hasNorm = false;
    bool hasCol = false;
    bool hasTex = false;
  
    std::vector<std::shared_ptr<hhMesh>> meshes;
    std::shared_ptr<hhMaterial> material;
    
    int numMeshes = 0;

    unsigned int dimensionality;

    glm::mat4 modelMatrix = glm::mat4(1.0);

    unsigned int numVertices = 0;
    unsigned int numIndices = 0;

    std::vector<float> combinedVertices;
    std::vector<unsigned int> combinedIndices;
    std::vector<std::vector<int>> flatMap; // i_th vector stores first index of each vertex from i_th mesh
    std::vector<int> meshOffsets; // meshOffsets[i] == index of first int in combinedIndices for i_th mesh


    hhModel(GLenum mode, int dimensionality) : modelVAO(), modelVBO(), modelIBO(), renderMode(mode), modelMatrix(glm::mat4(1.0f)), dimensionality(dimensionality) {};

    virtual void addMesh(const std::shared_ptr<hhMesh>& newMesh) = 0;
    std::shared_ptr<hhMesh> getMeshPtr(unsigned int i);

    void addMaterial(std::shared_ptr<hhMaterial>& newMaterial) { material = newMaterial; }

    virtual void checkUpdates() = 0;
    virtual void setBuffers() = 0;
    void updateBuffers();
    void renderModel();
    void renderModel(GLenum mode);

    void debugOutput();
};

std::shared_ptr<hhMesh> hhModel::getMeshPtr(unsigned int i) {
  if ((i < meshes.size()) && (meshes.size() != 0)) {
    return meshes[i];
  } else {
    std::cout << "meshes.size(): " << meshes.size() << std::endl;
    std::cout << "i: " << i << std::endl;
    std::cout << "getMeshPtr() index out of range" << std::endl;
    return nullptr;
  }
}

void hhModel::updateBuffers() {
  
  modelVAO.bind();

  modelVBO.bind();
  modelVBO.data(combinedVertices.data(), combinedVertices.size() * sizeof(float));

  modelVBO.unbind();
  modelVAO.unbind();

}

void hhModel::renderModel() {
  
  checkUpdates();

  if (bufferUpdateNeeded) {
    updateBuffers();
  }

  GLint currentProgram = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

  modelVAO.bind();
  glDrawElements(renderMode, numIndices, GL_UNSIGNED_INT, 0);
  modelVAO.unbind();
}
void hhModel::renderModel(GLenum mode) {

  checkUpdates();

  if (bufferUpdateNeeded) {
    updateBuffers();
  }

  modelVAO.bind();
  glDrawElements(mode, numIndices, GL_UNSIGNED_INT, 0);
  modelVAO.unbind();
}

//transformations


void hhModel::debugOutput() {
  std::cout << "*** hhModel debug ***" << std::endl;
  std::cout << "gl objects: " << "\n"
    << "VAO.ID: " << modelVAO.getID() << " | isBound: " << modelVAO.isBound() << "\n"
    << "VBO.ID: " << modelVBO.getID() << " | isBound: " << modelVBO.isBound() << "\n"
    << "IBO.ID: " << modelIBO.getID() << " | isBound: " << modelIBO.isBound() << std::endl;
  std::cout << " vertices & indices (max 64): " << std::endl;
  for (int i = 0; i < std::min(static_cast<int>(combinedIndices.size()), 64); i++) {
    std::cout << combinedIndices[i] << ":  "
      << "( " << combinedVertices[3 * combinedIndices[i]] << ", "
      << combinedVertices[3 * combinedIndices[i] + 1] << ", "
      << combinedVertices[3 * combinedIndices[i] + 2] << " )" << std::endl;
  }
}


#endif