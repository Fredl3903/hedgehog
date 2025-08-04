#ifndef BVH
#define BVH

#include <algorithm>

#include "../blinnPhongLayer.hpp"
#include "../../util/factory.hpp"
// #include "../../glObjects/compShader.hpp"


struct AABB {
  glm::vec3 min = glm::vec3(0.0);
  glm::vec3 max = glm::vec3(0.0);
  void fit(hhTri triangle);
};
void AABB::fit(hhTri t) {
  if (min == max) {
    min = t.centroid;
    max = t.centroid;
  }
  min = glm::min(min, glm::min(t.A, glm::min(t.B, t.C)));
  max = glm::max(max, glm::max(t.A, glm::max(t.B, t.C)));
  return;
}

struct node { // add padding for GPU alignment
  int leaf = 0;
  
  int firstTriI;
  int lastTriI;
  int numTris;
  
  glm::vec3 boxMin;
  glm::vec3 boxMax;

  int leftChildI = -1; // if internal node
  int rightChildI = -1; // if internal node

  int nodeIndex;
  int depth;
};

// ***************************************************************************************

class bvhLayer : public blinnPhongLayer {
  private:
  
    bool rayTrace = false;

  public:

    int numTris = 0;
    int maxDepth;
    bool built = false;

    std::vector<hhTri> tris; // all triangles in layer
    std::vector<unsigned int> triOffsets; // start indexes for each model in tris vector
    std::vector<glm::mat4> modelMatrices; // model matrix for each model

    std::vector<node> bvhNodes;

    bvhLayer(int depth, int zIndex) : blinnPhongLayer(zIndex), maxDepth(depth) {}

    void addModel(const std::shared_ptr<hhModel>& newModel) override;

    std::shared_ptr<hhModel3d> visualiseBVH();
    void addAABBMesh(std::shared_ptr<hhModel3d> model, int nodeIndex, int depth);

    void renderLayer() override;
    void enableRayTrace() { rayTrace = true; }
    void rayRender();

    void build();

  private:
    void tree(int currentNodeIndex, int depth);
    void split(int currentNodeIndex);
};

void bvhLayer::rayRender() {};

void bvhLayer::addModel(const std::shared_ptr<hhModel>& newModel) {
  if (newModel->dimensionality != dimensionality) {
    std::cout << "mismatching dimensionality in bvhLayer::addModel call" << std::endl;
    return;
  }
  
  models.push_back(newModel);
  numModels += 1;

  for (int i = 0; i < newModel->numMeshes; i++) {
    std::vector<hhTri> trisVec = newModel->getMeshPtr(i)->tris;
    if (trisVec.size() == 0) {
      newModel->getMeshPtr(i)->generateTris();
      trisVec = newModel->getMeshPtr(i)->tris;
    }

    for (int j = 0; j < trisVec.size(); j++) {
      tris.push_back(trisVec[j]);
      numTris++;
    }
  }
}

void bvhLayer::build() {

  node root;

  AABB rootBox;
  for (hhTri t : tris) {
    rootBox.fit(t);
  }
  root.boxMin = rootBox.min;
  root.boxMax = rootBox.max;

  root.firstTriI = 0;
  root.numTris = tris.size();
  root.lastTriI = tris.size();
  if (root.numTris == 1) {
    root.leaf = 1;
    bvhNodes.push_back(root);
    return;
  } else {
    root.leaf = 0;
  }
  root.nodeIndex = 0;
  bvhNodes.push_back(root);

  tree(root.nodeIndex, 0);

  std::cout << "done building" << std::endl;

}

void bvhLayer::tree(int currentNodeIndex, int depth) {

  bvhNodes[currentNodeIndex].depth = depth;
  if (depth >= maxDepth) {
    bvhNodes[currentNodeIndex].leaf = 1;
    return;
  }
  
  split(currentNodeIndex);

  if (bvhNodes[currentNodeIndex].leaf == 0) {
    tree(bvhNodes[currentNodeIndex].leftChildI, depth + 1);
    tree(bvhNodes[currentNodeIndex].rightChildI, depth + 1);
  }
}

void bvhLayer::split(int currentNodeIndex) {

  node& n = bvhNodes[currentNodeIndex];

  if (n.leaf == 1) return;

  float maxX = 0.0;
  float maxY = 0.0;
  float maxZ = 0.0;
  glm::vec3 minCentroid = tris[n.firstTriI].centroid;
  glm::vec3 maxCentroid = minCentroid;
  int splitAxis; // 0 - x; 1 - y; 2 - z

  for (int i = n.firstTriI + 1; i < n.lastTriI; i++) {
    glm::vec3 currentC = tris[i].centroid;
    if (currentC.x < minCentroid.x) { minCentroid.x = currentC.x; }
    if (currentC.y < minCentroid.y) { minCentroid.y = currentC.y; }
    if (currentC.z < minCentroid.z) { minCentroid.z = currentC.z; }
    if (currentC.x > maxCentroid.x) { maxCentroid.x = currentC.x; }
    if (currentC.y > maxCentroid.y) { maxCentroid.y = currentC.y; }
    if (currentC.z > maxCentroid.z) { maxCentroid.z = currentC.z; }
  }
  splitAxis = 0; // x axis
  if (maxCentroid.y - minCentroid.y > maxCentroid.x - minCentroid.x) {
    splitAxis = 1; // y axis
  }
  if (maxCentroid.z - minCentroid.z > maxCentroid.y - minCentroid.y) {
    splitAxis = 2; // z axis
  }

  std::nth_element(tris.begin() + n.firstTriI, tris.begin() + n.firstTriI + n.numTris / 2, 
    tris.begin() + n.lastTriI,
    [splitAxis](const hhTri& a, const hhTri& b) {
      if (splitAxis == 0) {
        return a.centroid.x < b.centroid.x;
      } else if (splitAxis == 1) {
        return a.centroid.y < b.centroid.y;
      } else {
        return a.centroid.z < b.centroid.z;
      }
    }
  );

  node leftChild;
  AABB leftBox;
  node rightChild;
  AABB rightBox;

  int mid = n.firstTriI + n.numTris / 2;

  leftChild.firstTriI = n.firstTriI;
  leftChild.lastTriI = mid;
  leftChild.numTris = mid - n.firstTriI;

  rightChild.firstTriI = mid;
  rightChild.lastTriI = n.lastTriI;
  rightChild.numTris = n.lastTriI - mid;

  if (leftChild.numTris <= 1) {
    leftChild.leaf = 1;
  }
  if (rightChild.numTris <= 1) {
    rightChild.leaf = 1;
  }

  for (int i = leftChild.firstTriI; i < leftChild.firstTriI + leftChild.numTris; i++) {
    leftBox.fit(tris[i]);
  }
  for (int i = rightChild.firstTriI; i < rightChild.firstTriI + rightChild.numTris; i++) {
    rightBox.fit(tris[i]);
  }
  leftChild.boxMin = leftBox.min;
  leftChild.boxMax = leftBox.max;
  rightChild.boxMin = rightBox.min;
  rightChild.boxMax = rightBox.max;

  leftChild.nodeIndex = bvhNodes.size();
  rightChild.nodeIndex = bvhNodes.size() + 1;
  
  n.leftChildI = leftChild.nodeIndex;
  n.rightChildI = rightChild.nodeIndex;
  
  bvhNodes[currentNodeIndex] = n;

  bvhNodes.push_back(leftChild);
  bvhNodes.push_back(rightChild);

}

void bvhLayer::addAABBMesh(std::shared_ptr<hhModel3d> model, int nodeIndex, int depth) {

  glm::vec3 min = bvhNodes[nodeIndex].boxMin;
  glm::vec3 max = bvhNodes[nodeIndex].boxMax;


  glm::vec3 boxColor = glm::mix(glm::vec3(0.0, 0.0, 0.7), glm::vec3(0.7, 0.0, 0.0), float(depth)/maxDepth);

  std::shared_ptr<hhMesh3d> newMesh = std::make_shared<hhMesh3d>(hhMeshFactory::makeAABB(min, max, boxColor));
  model->addMesh(newMesh);
  
  if (bvhNodes[nodeIndex].leaf == 0) {
    addAABBMesh(model, bvhNodes[nodeIndex].leftChildI, depth + 1);
    addAABBMesh(model, bvhNodes[nodeIndex].rightChildI, depth + 1);
  } else {
    return;
  }
}

std::shared_ptr<hhModel3d> bvhLayer::visualiseBVH() {
  std::shared_ptr<hhModel3d> bvhModel = std::make_shared<hhModel3d>(GL_LINES);
  addAABBMesh(bvhModel, 0, 0);
  return bvhModel;
}


void bvhLayer::renderLayer() {
  
  if (rayTrace) {

    rayRender();

    return;
  }
  basic3dLayer::renderLayer();
};

#endif