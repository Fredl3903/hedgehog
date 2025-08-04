#ifndef MESH
#define MESH

// each mesh should be generated with std::shared_ptr<hhMesh> mesh = std::make_shared<hhMesh>();
#include <C:/OpenGL/glm/glm/glm.hpp>
#include <vector>
#include <string>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ios>
#include <filesystem>
#include <any>

#include "../material/tex.hpp"
#include "../util/geometry.hpp"


class hhMesh {
  protected:

    virtual void makeAbstract() const = 0;
  
  public:


    unsigned int dimensionality;

    std::vector<hhVert> vertices;
    std::vector<hhTri> tris;
    std::vector<hhEdge> edges;
    
    std::vector<unsigned int> indices;

    hhMesh(int dimensions) : dimensionality(dimensions) {}
    ~hhMesh() {}

    void geometry(std::vector<hhVert> data, std::vector<unsigned int> indices);

    void generateTris();
    bool hasTris = false;
    std::vector<hhTri> getTris();

    bool updatedAll = false;
    std::vector<int> updatedIndices;
    // note that update functions should only change vertex properties, no changes to structure or indexing
    void update(int targetIndex, hhVert newVertex) { 
      vertices[targetIndex] = newVertex;
      updatedIndices.push_back(targetIndex);
    }
    void update(std::vector<hhVert> newVertexArray) { 
      if (newVertexArray.size() == vertices.size()) {
        vertices = newVertexArray;
        updatedAll = true;
      } else {
        std::cout << "mismatching vertex array sizes in update()" << std::endl;
      }
    }

    void setCols(glm::vec3 color);

    void addRes(int steps);

    int matchVertex(const hhVert& v) {
      for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i] == v) {
          return i;
        }
      }
      return -1;
    }
    int matchPosID(glm::vec3 pos) {
      for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i].pos == pos) {
          return vertices[i].posID;
        }
      }
      return -1;
    }
    int matchPos(glm::vec3 pos) {
      for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i].pos == pos) {
          return i;
        }
      }
      return -1;
    }
    int maxPosID() {
      int maxOut = 0;
      for(hhVert v : vertices) {
        if (v.posID > maxOut) {
          maxOut = v.posID;
        }
      }
      return maxOut;
    }
};

void hhMesh::geometry(std::vector<hhVert> data, std::vector<unsigned int> indices) {
  vertices = data;
  this->indices = indices;
}

void hhMesh::generateTris() {
  if (indices.size() % 3 != 0) {
    std::cout << "generateTris() failed - indices.size() not divisible by 3" << std::endl;
    return;
  }
  for (int i = 0; i < indices.size() / 3; i++) {
    hhTri newTriangle;
    
    newTriangle.A = vertices[indices[3 * i + 0]].pos;
    newTriangle.B = vertices[indices[3 * i + 1]].pos;
    newTriangle.C = vertices[indices[3 * i + 2]].pos;

    newTriangle.centroid = (newTriangle.A + newTriangle.B + newTriangle.C) / glm::vec3(3.0);

    tris.push_back(newTriangle);
  }
  hasTris = true;
}
std::vector<hhTri> hhMesh::getTris() {
  if (!hasTris) {
    generateTris();
  }
  return tris;
}

void hhMesh::setCols(glm::vec3 color) {
  for (hhVert& v : vertices) {
    v.col = color;
  }
  updatedAll = true;
}

void hhMesh::addRes(int steps) {

  for (int i = 0; i < steps; i++) {
    std::vector<unsigned int> tempI = indices;
    for (int tri = 0; tri < indices.size() / 3; tri++) {
      unsigned int triangle[3] = {tempI[tri * 3], tempI[tri * 3 + 1], tempI[tri * 3 + 2]};

      hhVert inter[3];
      unsigned int newIndices[3];
      
      inter[0].pos = glm::vec3(0.5) * (vertices[triangle[1]].pos + vertices[triangle[0]].pos);
      inter[0].norm = glm::vec3(0.5) * (vertices[triangle[1]].norm + vertices[triangle[0]].norm);
      inter[0].col = glm::vec3(0.5) * (vertices[triangle[1]].col + vertices[triangle[0]].col);
      inter[0].uv = glm::vec2(0.5) * (vertices[triangle[1]].uv + vertices[triangle[0]].uv);

      inter[1].pos = glm::vec3(0.5) * (vertices[triangle[2]].pos + vertices[triangle[1]].pos);
      inter[1].norm = glm::vec3(0.5) * (vertices[triangle[2]].norm + vertices[triangle[1]].norm);
      inter[1].col = glm::vec3(0.5) * (vertices[triangle[2]].col + vertices[triangle[1]].col);
      inter[1].uv = glm::vec2(0.5) * (vertices[triangle[2]].uv + vertices[triangle[1]].uv);

      inter[2].pos = vertices[triangle[2]].pos + glm::vec3(0.5) * (vertices[triangle[0]].pos - vertices[triangle[2]].pos);
      inter[2].norm = glm::vec3(0.5) * (vertices[triangle[0]].norm + vertices[triangle[2]].norm);
      inter[2].col = glm::vec3(0.5) * (vertices[triangle[0]].col + vertices[triangle[2]].col);
      inter[2].uv = glm::vec2(0.5) * (vertices[triangle[0]].uv + vertices[triangle[2]].uv);

      for (int v = 0; v < 3; v++) {
        int dupCheck = matchVertex(inter[v]);
        if (dupCheck == -1) {
          newIndices[v] = vertices.size();
          int posCheck = matchPosID(inter[v].pos);
          if (posCheck == -1) { inter[v].posID = maxPosID() + 1; }
          else { inter[v].posID = posCheck; }
          vertices.push_back(inter[v]);
        } else {
          newIndices[v] = dupCheck;
        }
      }

      tempI.push_back(triangle[0]);
      tempI.push_back(newIndices[0]);
      tempI.push_back(newIndices[2]);
      tempI.push_back(newIndices[0]);
      tempI.push_back(triangle[1]);
      tempI.push_back(newIndices[1]);
      tempI.push_back(newIndices[2]);
      tempI.push_back(newIndices[1]);
      tempI.push_back(triangle[2]);
      tempI.push_back(newIndices[2]);
      tempI.push_back(newIndices[0]);
      tempI.push_back(newIndices[1]);
    } 
    indices = tempI;
  }
}

#endif