#ifndef MODEL_3D
#define MODEL_3D

#include "model.hpp"
#include "mesh3d.hpp"

class hhModel3d : public hhModel {

  public:

    hhModel3d(GLenum mode) : hhModel(mode, 3) {}

    void addMesh(const std::shared_ptr<hhMesh>& newMesh) override;

    void translate(glm::vec3 move);
    void rotate(glm::vec3 pivot, glm::vec3 axis, float angle);
    void scale(glm::vec3 center, glm::vec3 scaleFactors);

    void checkUpdates() override;
    void setBuffers() override;
};

void hhModel3d::addMesh(const std::shared_ptr<hhMesh>& newMesh) {

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
  
  unsigned int dataLength = 0;
  
  std::vector<int> newFlatMap;
    
  dataLength = newMesh->vertices.size();

  hasNorm = true;
  hasCol = true;

  for (int i = 0; i < dataLength; i++) {

    newFlatMap.push_back(combinedVertices.size());

    combinedVertices.push_back(newMesh->vertices[i].pos.x);
    combinedVertices.push_back(newMesh->vertices[i].pos.y);
    combinedVertices.push_back(newMesh->vertices[i].pos.z);

    combinedVertices.push_back(newMesh->vertices[i].norm.x);
    combinedVertices.push_back(newMesh->vertices[i].norm.y);
    combinedVertices.push_back(newMesh->vertices[i].norm.z);

    combinedVertices.push_back(newMesh->vertices[i].col.x);
    combinedVertices.push_back(newMesh->vertices[i].col.y);
    combinedVertices.push_back(newMesh->vertices[i].col.z);

    combinedVertices.push_back(newMesh->vertices[i].uv.x);
    combinedVertices.push_back(newMesh->vertices[i].uv.y);

  }

  numVertices += dataLength;
  flatMap.push_back(newFlatMap);

  // for (int i = 0; i < combinedIndices.size(); i++) {
  //   std::cout << "combinedIndices[i] == " << combinedIndices[i] << std::endl;
  // }
}


void hhModel3d::checkUpdates() {
  bufferUpdateNeeded = false;

  for (int i = 0; i < numMeshes; i++) {

    std::shared_ptr<hhMesh> currentMesh = meshes[i];

    if (currentMesh->updatedAll) {
      for (int j = 0; j < (currentMesh->vertices).size(); j++) {
        int nextUpdateIndex = flatMap[i][j];

        combinedVertices[nextUpdateIndex] = (currentMesh->vertices)[j].pos.x;
        combinedVertices[nextUpdateIndex + 1] = (currentMesh->vertices)[j].pos.y;
        combinedVertices[nextUpdateIndex + 2] = (currentMesh->vertices)[j].pos.z;

        combinedVertices[nextUpdateIndex + 3] = (currentMesh->vertices)[j].norm.x;
        combinedVertices[nextUpdateIndex + 4] = (currentMesh->vertices)[j].norm.y;
        combinedVertices[nextUpdateIndex + 5] = (currentMesh->vertices)[j].norm.z;

        combinedVertices[nextUpdateIndex + 6] = (currentMesh->vertices)[j].col.x;
        combinedVertices[nextUpdateIndex + 7] = (currentMesh->vertices)[j].col.y;
        combinedVertices[nextUpdateIndex + 8] = (currentMesh->vertices)[j].col.z;

        combinedVertices[nextUpdateIndex + 9] = (currentMesh->vertices)[j].uv.x;
        combinedVertices[nextUpdateIndex + 10] = (currentMesh->vertices)[j].uv.y;
      } 
      currentMesh->updatedAll = false;
      bufferUpdateNeeded = true;
    } else {
      if ((currentMesh->updatedIndices).size() != 0) {
        for (int j : currentMesh->updatedIndices) {
          int nextUpdateIndex = flatMap[i][j];

          combinedVertices[nextUpdateIndex] = (currentMesh->vertices)[j].pos.x;
          combinedVertices[nextUpdateIndex + 1] = (currentMesh->vertices)[j].pos.y;
          combinedVertices[nextUpdateIndex + 2] = (currentMesh->vertices)[j].pos.z;

          combinedVertices[nextUpdateIndex + 3] = (currentMesh->vertices)[j].norm.x;
          combinedVertices[nextUpdateIndex + 4] = (currentMesh->vertices)[j].norm.y;
          combinedVertices[nextUpdateIndex + 5] = (currentMesh->vertices)[j].norm.z;

          combinedVertices[nextUpdateIndex + 6] = (currentMesh->vertices)[j].col.x;
          combinedVertices[nextUpdateIndex + 7] = (currentMesh->vertices)[j].col.y;
          combinedVertices[nextUpdateIndex + 8] = (currentMesh->vertices)[j].col.z;

          combinedVertices[nextUpdateIndex + 9] = (currentMesh->vertices)[j].uv.x;
          combinedVertices[nextUpdateIndex + 10] = (currentMesh->vertices)[j].uv.y;
        }
        bufferUpdateNeeded = true;
      }
      currentMesh->updatedIndices.clear();
    }
  }
}

void hhModel3d::setBuffers() {
    
  modelVAO.bind();

  modelVBO.bind();
  modelVBO.data(combinedVertices.data(), combinedVertices.size() * sizeof(float));

  modelIBO.bind();
  modelIBO.data(combinedIndices.data(), combinedIndices.size() * sizeof(unsigned int));


  // base stride for required position data
  unsigned int stride = 3 * sizeof(float);

  stride += 3 * sizeof(float); // norm 
  stride += 3 * sizeof(float); // col
  stride += 2 * sizeof(float); // tex

  std::uintptr_t offset = 0;
  modelVAO.addAttrib(0, dimensionality, GL_FLOAT, stride, (void*)offset);
  
  offset += 3 * sizeof(float);
  modelVAO.addAttrib(1, 3, GL_FLOAT, stride, (void*)offset);
  
  offset += 3 * sizeof(float);
  modelVAO.addAttrib(2, 3, GL_FLOAT, stride, (void*)offset);
  
  offset += 3 * sizeof(float);
  modelVAO.addAttrib(3, 2, GL_FLOAT, stride, (void*)offset);

  modelVBO.unbind();
  modelIBO.unbind();
  modelVAO.unbind();
}

void hhModel3d::translate(glm::vec3 move) {
    modelMatrix = glm::translate(modelMatrix, move);
}
void hhModel3d::rotate(glm::vec3 pivot, glm::vec3 axis, float angle) {
    translate(-pivot);
    modelMatrix = glm::rotate(modelMatrix, angle, axis);
    translate(pivot);
}
void hhModel3d::scale(glm::vec3 center, glm::vec3 scaleFactors) {
    modelMatrix = glm::scale(modelMatrix, scaleFactors);
}

#endif