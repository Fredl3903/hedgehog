#include <vector>
#include <memory>

#include <C:/OpenGL/glm/glm/glm.hpp>
#include <C:/OpenGL/GLEW/include/GL/glew.h>

#include "mesh.hpp"
#include "../glObjects/VAO.hpp"

class hhModel {

  private:

    std::vector<std::weak_ptr<hhMesh>> meshes;
    VAO modelVAO;
    VBO modelVBO;
    IBO modelIBO;

    int numMeshes = 0;
    unsigned int dimensionality = 2;

    bool hasNorm = false;
    bool hasCol = false;
    bool hasTex = false;
  
  public:

    std::vector<float> combinedVertices;
    std::vector<unsigned int> combinedIndices;
    unsigned int numVertices = 0;
    unsigned int numIndices = 0;

    GLenum renderMode; // same for all meshes

    glm::mat4 modelMatrix = glm::mat4(1.0);

    hhModel(GLenum mode) : modelVAO(), modelVBO(), modelIBO(), renderMode(mode), modelMatrix(glm::mat4(1.0f)) {};

    // note: all meshes in a model must have the same layout.
    void addMesh(const hhMesh& newMesh);

    void translate(glm::vec3 move);
    void translate(glm::vec2 move);
    void rotate(glm::vec3 pivot, glm::vec3 axis, float angle);
    void rotate(glm::vec2 pivot, float angle);
    void scale(glm::vec3 center, glm::vec3 scaleFactors);
    void scale(glm::vec2 center, glm::vec2 scaleFactors);

    void setBuffers();
    void renderModel();


    void debugOutput();
};

void hhModel::addMesh(const hhMesh& newMesh) {

  if (meshes.size() == 0) {
      dimensionality = newMesh.dimensionality;
  } else if (dimensionality != newMesh.dimensionality) {
      throw std::invalid_argument("new mesh has mismatching dimensionality");
  } else if ((hasNorm != (newMesh.data.count("norm") > 0) || hasCol != (newMesh.data.count("col") > 0)) ||
  hasTex != (newMesh.data.count("tex") > 0)) {
      throw std::invalid_argument("new mesh has mismatching layout");
  }

  meshes.push_back(std::make_shared<hhMesh>(newMesh));
  numMeshes += 1;

  // add indices first to use old numVertices for adjusting
  if (newMesh.data.count("indices") > 0) {
      std::vector<unsigned int> tempIndices = *(newMesh.data.at("indices").indices);
      for (int i = 0; i < tempIndices.size(); i++) {
          tempIndices[i] += numVertices;
          combinedIndices.push_back(tempIndices[i]);
      }
      numIndices += tempIndices.size();
  }

  std::vector<float> tempPos = *(newMesh.data.at("pos").dataVec);
  if (tempPos.size() % 3 != 0) {
      throw std::length_error("invalid data array");
  }
  unsigned int dataLength = tempPos.size() / 3;

  std::vector<float> tempNorm;
  std::vector<float> tempCol;
  std::vector<float> tempTex;
  if (newMesh.data.count("norm") > 0) {
    std::cout << "adding norms" << std::endl;
    hasNorm = true;
    tempNorm = *(newMesh.data.at("norm").dataVec);
  }
  if (newMesh.data.count("col") > 0) {
    std::cout << "adding cols" << std::endl;
    hasCol = true;
    tempCol = *(newMesh.data.at("col").dataVec);
  }
  if (newMesh.data.count("tex") > 0) {
    std::cout << "adding tex" << std::endl;
    hasTex = true;
    tempTex = *(newMesh.data.at("tex").dataVec);
  }

  for (int i = 0; i < dataLength; i++) {
    for (int j = 0; j < dimensionality; j++) {
      combinedVertices.push_back(tempPos[3 * i + j]);    
    }
    if (hasNorm) {
      for (int j = 0; j < 3; j++) {
        combinedVertices.push_back(tempNorm[3 * i + j]);    
      }
    }
    if (hasCol) {
      for (int j = 0; j < 3; j++) {
        combinedVertices.push_back(tempCol[3 * i + j]);    
      }
    }
    if (hasTex) {
      for (int j = 0; j < 2; j++) {
        combinedVertices.push_back(tempTex[3 * i + j]);    
      }
    }
  }
  numVertices += dataLength;
}

void hhModel::setBuffers() {
    
    modelVAO.bind();

    modelVBO.bind();
    modelVBO.data(combinedVertices.data());
    modelVBO.unbind();

    modelIBO.bind();
    modelIBO.data(combinedIndices.data());
    modelIBO.unbind();


    // base stride for required position data
    unsigned int stride = dimensionality * sizeof(float);

    if (hasNorm) { stride += 3 * sizeof(float); }
    if (hasCol) { stride += 3 * sizeof(float); }
    if (hasTex) { stride += 2 * sizeof(float); }

    std::uintptr_t offset = 0;
    
    modelVAO.addAttrib(0, dimensionality, GL_FLOAT, stride, (void*)offset);
    
    if (hasNorm) { offset += 3 * sizeof(float);
    modelVAO.addAttrib(1, 3, GL_FLOAT, stride, (void*)offset); }
    
    if (hasCol) { offset += 3 * sizeof(float);
    modelVAO.addAttrib(2, 3, GL_FLOAT, stride, (void*)offset); }
    
    if (hasTex) { offset += 3 * sizeof(float);
    modelVAO.addAttrib(3, 2, GL_FLOAT, stride, (void*)offset); }

    modelVAO.unbind();
}

void hhModel::renderModel() {
  // std::cout << "called renderModel: " << std::endl;
  // std::vector<float> checkPos = combinedVertices;
  // for (int i = 0; i < checkPos.size()/3; i++) {
  //   std::cout << "(" << checkPos[3 * i] << ", " <<
  //     checkPos[3 * i + 1] << ", " <<
  //     checkPos[3 * i + 2] << ")" << std::endl;
  //   std::cout << "numIndices: " << numIndices << std::endl;
  // }
  // std::cout << "*******************************************************************************" << std::endl;
  modelVAO.bind();
  glDrawElements(renderMode, numIndices, GL_UNSIGNED_INT, 0);
  modelVAO.unbind();

}


//transformations

void hhModel::translate(glm::vec3 move) {
    modelMatrix = glm::translate(modelMatrix, move);
}
void hhModel::translate(glm::vec2 move) {
    modelMatrix = glm::translate(modelMatrix, glm::vec3(move.x, move.y, 0.0));
}
void hhModel::rotate(glm::vec3 pivot, glm::vec3 axis, float angle) {
    translate(-pivot);
    modelMatrix = glm::rotate(modelMatrix, angle, axis);
    translate(pivot);
}
void hhModel::rotate(glm::vec2 pivot, float angle) {
    translate(-pivot);
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0, 0.0, 1.0));
    translate(pivot);
}
void hhModel::scale(glm::vec3 center, glm::vec3 scaleFactors) {
    modelMatrix = glm::scale(modelMatrix, scaleFactors);
}
void hhModel::scale(glm::vec2 center, glm::vec2 scaleFactors) {
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactors.x, scaleFactors.y, 0.0));
}




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