#ifndef MODEL_2D
#define MODEL_2D

#include "model.hpp"
#include "mesh2d.hpp"

class hhModel2d : public hhModel {

  public:

    hhModel2d(GLenum mode) : hhModel(mode, 2) {}

    void addMesh(const std::shared_ptr<hhMesh>& newMesh) override;

    void translate(glm::vec2 move);
    void rotate(glm::vec2 pivot, float angle);
    void scale(glm::vec2 center, glm::vec2 scaleFactors);

    void checkUpdates() override;
    void setBuffers() override;
};

void hhModel2d::addMesh(const std::shared_ptr<hhMesh>& newMesh) {

  if (dimensionality != newMesh->dimensionality) {
    throw std::invalid_argument("new mesh has mismatching dimensionality");
  }

  meshes.push_back(newMesh);
  numMeshes += 1;

  // add indices first to use old numVertices for adjusting
  std::vector<unsigned int> tempIndices = newMesh->indices;
  for (int i = 0; i < newMesh->indices.size(); i++) {
    tempIndices[i] += numVertices;
    combinedIndices.push_back(tempIndices[i]);
  }
  meshOffsets.push_back(numIndices);
  numIndices += tempIndices.size();
  
  unsigned int dataLength = newMesh->vertices.size();
  std::vector<int> newFlatMap;

  hasNorm = false;
  hasCol = true;

  for (int i = 0; i < dataLength; i++) {

    newFlatMap.push_back(combinedVertices.size());

    combinedVertices.push_back(newMesh->vertices[i].pos.x);
    combinedVertices.push_back(newMesh->vertices[i].pos.y);

    combinedVertices.push_back(newMesh->vertices[i].col.x);
    combinedVertices.push_back(newMesh->vertices[i].col.y);
    combinedVertices.push_back(newMesh->vertices[i].col.z);

    combinedVertices.push_back(newMesh->vertices[i].uv.x);
    combinedVertices.push_back(newMesh->vertices[i].uv.y);

  }

  numVertices += dataLength;
  flatMap.push_back(newFlatMap);

  for (float x : combinedVertices) {
    std::cout << x << std::endl;
  }
}

void hhModel2d::checkUpdates() {
  bufferUpdateNeeded = false;

  for (int i = 0; i < numMeshes; i++) {

    std::shared_ptr<hhMesh> currentMesh = meshes[i];

    if (currentMesh->updatedAll) {
      for (int j = 0; j < (currentMesh->vertices).size(); j++) {
        int nextUpdateIndex = flatMap[i][j];

        combinedVertices[nextUpdateIndex] = (currentMesh->vertices)[j].pos.x;
        combinedVertices[nextUpdateIndex + 1] = (currentMesh->vertices)[j].pos.y;

        combinedVertices[nextUpdateIndex + 2] = (currentMesh->vertices)[j].col.x;
        combinedVertices[nextUpdateIndex + 3] = (currentMesh->vertices)[j].col.y;
        combinedVertices[nextUpdateIndex + 4] = (currentMesh->vertices)[j].col.z;

        combinedVertices[nextUpdateIndex + 5] = (currentMesh->vertices)[j].uv.x;
        combinedVertices[nextUpdateIndex + 6] = (currentMesh->vertices)[j].uv.y;
      } 
      currentMesh->updatedAll = false;
      bufferUpdateNeeded = true;
    } else {
      if ((currentMesh->updatedIndices).size() != 0) {
        for (int j : currentMesh->updatedIndices) {
          int nextUpdateIndex = flatMap[i][j];

          combinedVertices[nextUpdateIndex] = (currentMesh->vertices)[j].pos.x;
          combinedVertices[nextUpdateIndex + 1] = (currentMesh->vertices)[j].pos.y;

          combinedVertices[nextUpdateIndex + 2] = (currentMesh->vertices)[j].col.x;
          combinedVertices[nextUpdateIndex + 3] = (currentMesh->vertices)[j].col.y;
          combinedVertices[nextUpdateIndex + 4] = (currentMesh->vertices)[j].col.z;

          combinedVertices[nextUpdateIndex + 5] = (currentMesh->vertices)[j].uv.x;
          combinedVertices[nextUpdateIndex + 6] = (currentMesh->vertices)[j].uv.y;
        }
        bufferUpdateNeeded = true;
      }
      currentMesh->updatedIndices.clear();
    }
  }
}

void hhModel2d::setBuffers() {
    
  if (dimensionality != 2) {
    std::cout << "setBuffers2d called on non-2d model" << std::endl;
    return;
  }

  modelVAO.bind();

  modelVBO.bind();
  modelVBO.data(combinedVertices.data(), combinedVertices.size() * sizeof(float));

  modelIBO.bind();
  modelIBO.data(combinedIndices.data(), combinedIndices.size() * sizeof(unsigned int));


  // base stride for required position data
  unsigned int stride = 2 * sizeof(float);
  stride += 3 * sizeof(float); // col
  stride += 2 * sizeof(float); // tex

  std::uintptr_t offset = 0;
  modelVAO.addAttrib(0, dimensionality, GL_FLOAT, stride, (void*)offset);
  
  offset += 2 * sizeof(float);
  modelVAO.addAttrib(1, 3, GL_FLOAT, stride, (void*)offset);
  
  offset += 3 * sizeof(float);
  modelVAO.addAttrib(2, 2, GL_FLOAT, stride, (void*)offset);

  modelVBO.unbind();
  modelIBO.unbind();
  modelVAO.unbind();
}

void hhModel2d::translate(glm::vec2 move) {
    modelMatrix = glm::translate(modelMatrix, glm::vec3(move.x, move.y, 0.0));
}
void hhModel2d::rotate(glm::vec2 pivot, float angle) {
    translate(-pivot);
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0, 0.0, 1.0));
    translate(pivot);
}
void hhModel2d::scale(glm::vec2 center, glm::vec2 scaleFactors) {
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactors.x, scaleFactors.y, 0.0));
}


#endif