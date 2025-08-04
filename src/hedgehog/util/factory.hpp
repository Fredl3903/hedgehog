#ifndef FACTORY
#define FACTORY

#include "../model/mesh3d.hpp"
#include "../model/mesh2d.hpp"

class hhMeshFactory {

  public:

    // 3d primitives
    static hhMesh3d makeCube(float size);
    static hhMesh3d makeIcos(float radius);
    static hhMesh3d makeIcoSphere(float radius, int resolution);
    static hhMesh3d makeSphere(float radius, int resolution);
    static hhMesh3d makeCylinder(float radius, float length, int resolution);
    static hhMesh3d makePlane(glm::vec3 UL, glm::vec3 UR, glm::vec3 BL);

    // 2d primitives
    static hhMesh2d makeQuad(glm::vec2 min, glm::vec2 max);

    // indexed for wireframe
    static hhMesh3d makeAABB(glm::vec3 min, glm::vec3 max, glm::vec3 col);

    // particles
    static hhMesh3d makeGridAlignedParticles(glm::vec3 min, glm::vec3 max, glm::vec3 res);
    
};


hhMesh3d hhMeshFactory::makeCube(float size) {

  float half = size / 2.0;

  std::vector<glm::vec3> pos = {
    glm::vec3(-half, -half, half), glm::vec3(-half, -half, -half), glm::vec3(-half, half, -half), glm::vec3(-half, half, half), //left
    glm::vec3(-half, -half, -half), glm::vec3(half, -half, -half), glm::vec3(half, half, -half), glm::vec3(-half, half, -half), //back
    glm::vec3(half, -half, -half), glm::vec3(half, -half, half), glm::vec3(half, half, half), glm::vec3(half, half, -half), //right
    glm::vec3(half, -half, half), glm::vec3(-half, -half, half), glm::vec3(-half, half, half), glm::vec3(half, half, half), //front
    glm::vec3(half, -half, -half), glm::vec3(-half, -half, -half), glm::vec3(-half, -half, half), glm::vec3(half, -half, half), //bottom
    glm::vec3(half, half, half), glm::vec3(-half, half, half), glm::vec3(-half, half, -half), glm::vec3(half, half, -half), //top
  };
  std::vector<glm::vec3> norm = {
    glm::vec3(-1.0, 0.0, 0.0), //left
    glm::vec3(0.0, 0.0, -1.0), //back
    glm::vec3(1.0, 0.0, 0.0), //right
    glm::vec3(0.0, 0.0, 1.0), //front
    glm::vec3(0.0, -1.0, 0.0), //bottom
    glm::vec3(0.0, 1.0, 0.0), //top
  };
  glm::vec3 col(1.0);
  std::vector<glm::vec2> uv = {
    glm::vec2(1.0, 0.0),
    glm::vec2(0.0, 0.0),
    glm::vec2(0.0, 1.0),
    glm::vec2(1.0, 1.0)
  };

  std::vector<hhVert> data(24);
  for (int i = 0; i < 24; i++) {
    data[i].pos = pos[i];
    data[i].col = col;
  }
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 4; j++) {
      data[4 * i + j].norm = norm[i];
      data[4 * i + j].uv = uv[j];
    }
  }

  std::vector<unsigned int> indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20
  };

  return hhMesh3d(std::move(data), std::move(indices));

}

hhMesh3d hhMeshFactory::makeIcos(float R) {

  float gold = (1.0 + glm::sqrt(5)) / 2.0;
  // std::cout << "golden ratio = " << gold << "    [store by precalculated value]" << std::endl;
  // std::cout << "sqrt(gold^2 + 1) = " << glm::sqrt(gold * gold + 1) << "    [store by precalculated value]" << std::endl;
  float A = R / glm::sqrt(gold * gold + 1); // scaled from 1
  float B = A * gold; // scaled from golden ratio

  std::vector<glm::vec3> posNorm = {
    glm::vec3(0.0, A, B),       glm::vec3(B, 0.0, A),       glm::vec3(A, B, 0.0),
    glm::vec3(0.0, -A, B),      glm::vec3(B, 0.0, -A),      glm::vec3(-A, B, 0.0),
    glm::vec3(0.0, A, -B),      glm::vec3(-B, 0.0, A),      glm::vec3(A, -B, 0.0),
    glm::vec3(0.0, -A, -B),     glm::vec3(-B, 0.0, -A),     glm::vec3(-A, -B, 0.0),
  };
  glm::vec3 col(1.0);
  glm::vec2 uvBlank(1.0);

  std::vector<hhVert> vertices(12);
    for (int i = 0; i < 12; i++) {
    vertices[i].pos = posNorm[i];
    vertices[i].norm = glm::normalize(posNorm[i]);
    vertices[i].col = col;
    vertices[i].uv = uvBlank;
  }

  std::vector<unsigned int> indices = {
    0,  1,  2,
    0,  2,  5,
    0,  5, 7,
    0, 7, 3,
    0, 3,  1,
    1, 3,  8,
    1, 8,  4,
    1, 4,  2,
    2, 4,  6,
    2, 6,  5,
    5, 6, 10,
    5, 10, 7,
    7, 10, 11,
    7, 11,  3,
    3, 11,  8,
    9, 8,  11,
    9, 4,  8,
    9, 6,  4,
    9, 10, 6,
    9, 11, 10
  };

  return hhMesh3d(std::move(vertices), std::move(indices));
}

hhMesh3d hhMeshFactory::makeIcoSphere(float R, int res) {

  // res = number of interpolations.

  hhMesh3d base = makeIcos(1.0);
  base.addRes(res);

  for (int v = 0; v < base.vertices.size(); v++) {
    base.vertices[v].pos *= glm::vec3(R / glm::length(base.vertices[v].pos)); 
  }

  return hhMesh3d(std::move(base.vertices), std::move(base.indices));

}

hhMesh3d hhMeshFactory::makeSphere(float R, int res) {


  hhMesh3d baseCube = makeCube(R);
  if (res > 1) { baseCube.addRes(res); }

  for (hhVert& v : baseCube.vertices) {
    v.pos *= glm::vec3(R) / glm::length(v.pos);
    v.norm = v.pos / glm::vec3(R);
  }

  return hhMesh3d(baseCube);
}

hhMesh3d hhMeshFactory::makeCylinder(float R, float L, int res) {
  
  std::vector<hhVert> vertices;
  std::vector<unsigned int> indices;

  if (res < 3) {
    std::cerr << "cylinder resolution must be > 2" << std::endl;
    return hhMesh3d(std::vector<hhVert>(0), std::vector<unsigned int>(0));
  }

  hhVert v0;
  v0.pos = glm::vec3(0.0);
  v0.norm = glm::vec3(0.0, -1.0, 0.0);
  v0.col = glm::vec3(1.0);
  v0.uv = glm::vec2(0.0);
  vertices.push_back(v0);

  for (int i = 0; i < res; i++) {
    hhVert v;
    v.pos = glm::vec3(R * cos(i * (glm::radians(360.0 / res))), 0, R * sin(i * (glm::radians(360.0 / res))));
    v.norm = glm::vec3(0.0, -1.0, 0.0);
    v.col = glm::vec3(1.0);
    v.uv = glm::vec2(0.0);
    v.posID = i + 1;
    vertices.push_back(v);
  }
  for (int i = 0; i < res; i++) {
    hhVert v1;
    hhVert v2;

    v1.pos = glm::vec3(R * cos(i * glm::radians(360.0 / res)), 0, R * sin(i * glm::radians(360.0 / res)));
    v2.pos = glm::vec3(R * cos(i * glm::radians(360.0 / res)), L, R * sin(i * glm::radians(360.0 / res)));

    v1.norm = glm::vec3(R * cos(i * glm::radians(360.0 / res)), 0, R * sin(i * glm::radians(360.0 / res)));
    v2.norm = glm::vec3(R * cos(i * glm::radians(360.0 / res)), 0, R * sin(i * glm::radians(360.0 / res)));

    v1.col = glm::vec3(1.0);
    v2.col = glm::vec3(1.0);

    v1.uv = glm::vec2(0.0);
    v2.uv = glm::vec2(0.0);

    v1.posID = i + 1;
    v2.posID = i + 1 + res;

    vertices.push_back(v1);
    vertices.push_back(v2);
  }
  for (int i = 0; i < res; i++) {
    hhVert v;
    v.pos = glm::vec3(R * cos(i * glm::radians(360.0 / res)), L, R * sin(i * glm::radians(360.0 / res)));
    v.norm = glm::vec3(0.0, 1.0, 0.0);
    v.col = glm::vec3(1.0);
    v.uv = glm::vec2(0.0);
    v.posID = i + 1 + res;
    vertices.push_back(v);
  }

  hhVert vEnd;
  vEnd.pos = glm::vec3(0.0, L, 0.0);
  vEnd.norm = glm::vec3(0.0, 1.0, 0.0);
  vEnd.col = glm::vec3(1.0);
  vEnd.uv = glm::vec2(0.0);
  vertices.push_back(vEnd);



  for (int i = 0; i < res - 1; i++) {
    indices.push_back(0);
    indices.push_back(i + 1);
    indices.push_back(i + 2);
  }
  indices.push_back(0);
  indices.push_back(res);
  indices.push_back(1);

  int firstSideIndex = res + 1;
  for (int i = 0; i < res - 1; i++) {
    indices.push_back(firstSideIndex + 2 * i);
    indices.push_back(firstSideIndex + 2 * i + 1);
    indices.push_back(firstSideIndex + 2 * i + 3);
    indices.push_back(firstSideIndex + 2 * i + 3);
    indices.push_back(firstSideIndex + 2 * i + 2);
    indices.push_back(firstSideIndex + 2 * i);
  }
  indices.push_back(firstSideIndex + 2 * res - 2);
  indices.push_back(firstSideIndex + 2 * res - 1);
  indices.push_back(firstSideIndex + 1);
  indices.push_back(firstSideIndex + 1);
  indices.push_back(firstSideIndex);
  indices.push_back(firstSideIndex + 2 * res - 2);

  int end = vertices.size() - 1;
  for (int i = 0; i < res - 1; i++) {
    indices.push_back(end);
    indices.push_back(end - res + i);
    indices.push_back(end - res + i + 1);
  }
  indices.push_back(end);
  indices.push_back(end - 1);
  indices.push_back(end - res);

  return hhMesh3d(std::move(vertices), std::move(indices));

}

hhMesh3d hhMeshFactory::makePlane(glm::vec3 UL, glm::vec3 UR, glm::vec3 BL) {

  std::vector<glm::vec3> pos = {
    UL, UR, BL, (BL + UR - UL)
  };
  glm::vec3 norm = glm::cross((BL - UL), (UR - UL));
  glm::vec3 col(1.0);
  std::vector<glm::vec2> uv = {
    glm::vec2(0.0, 1.0),
    glm::vec2(1.0, 1.0),
    glm::vec2(0.0, 0.0),
    glm::vec2(1.0, 0.0)
  };

  std::vector<hhVert> data(4);
  for (int i = 0; i < 4; i++) {
    data[i].pos = pos[i];
    data[i].norm = norm;
    data[i].col = col;
    data[i].uv = uv[i];
  }

  std::vector<unsigned int> indices = {
    0, 1, 2, 1, 2, 3
  };

  return hhMesh3d(std::move(data), std::move(indices));

}


hhMesh2d hhMeshFactory::makeQuad(glm::vec2 min, glm::vec2 max) {

  std::vector<glm::vec3> pos = {
    glm::vec3(min.x, min.y, 0.0), glm::vec3(min.x, max.y, 0.0),
    glm::vec3(max.x, max.y, 0.0), glm::vec3(max.x, min.y, 0.0)
  };
  glm::vec3 col(1.0);
  std::vector<glm::vec2> uv = {
    glm::vec2(0.0, 0.0),
    glm::vec2(0.0, 1.0),
    glm::vec2(1.0, 1.0),
    glm::vec2(1.0, 0.0)
  };

  std::vector<hhVert> data(4);
  for (int i = 0; i < 4; i++) {
    data[i].pos = pos[i];
    data[i].col = col;
    data[i].uv = uv[i];
  }

  std::vector<unsigned int> indices = {
    0, 1, 2, 0, 2, 3
  };

  return hhMesh2d(std::move(data), std::move(indices));

}



hhMesh3d hhMeshFactory::makeAABB(glm::vec3 min, glm::vec3 max, glm::vec3 col) {

  std::vector<glm::vec3> pos = {
    glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, max.y, max.z), //left
    glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z) //right
  };

  std::vector<hhVert> data(8);
  for (int i = 0; i < 8; i++) {
    data[i].pos = pos[i];
    data[i].col = col;
    data[i].norm = glm::vec3(1.0);
    data[i].uv = glm::vec2(1.0);
  }

  std::vector<unsigned int> indices = {
    0, 1, 1, 2, 2, 3, 3, 0,
    1, 5, 5, 6, 6, 2, 2, 1,
    4, 5, 5, 6, 6, 7, 7, 4,
    0, 4, 4, 7, 7, 3, 3, 0,
    3, 2, 2, 6, 6, 7, 7, 3,
    0, 1, 1, 5, 5, 4, 4, 0
  };

  return hhMesh3d(std::move(data), std::move(indices));

}

hhMesh3d hhMeshFactory::makeGridAlignedParticles(glm::vec3 min, glm::vec3 max, glm::vec3 res) {
  
  int numPts = res.x * res.y * res.z;
  float dx = (max.x - min.x) / (res.x - 1);
  float dy = (max.y - min.y) / (res.y - 1);
  float dz = (max.z - min.z) / (res.z - 1);

  std::vector<hhVert> pts(numPts);
  for (int i = 0; i < res.x; i++) {
    for (int j = 0; j < res.y; j++) {
      for (int k = 0; k < res.z; k++) {
        pts[i * res.y * res.z + j * res.z + k].pos = min + glm::vec3(i * dx, j * dy, k * dz);
      }
    }
  }
  std::vector<unsigned int> indices;
  for (int p = 0; p < numPts; p++) {
    indices.push_back(p);
  }

  return hhMesh3d(std::move(pts), std::move(indices));
}


#endif