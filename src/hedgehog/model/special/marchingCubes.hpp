#ifndef MARCHING_CUBES
#define MARCHING_CUBES

#include "../mesh.hpp"
#include "../../shader/compShader.hpp"

struct gridPoint {
  glm::vec3 pos;
  float val;
};

class cubesMesh : public hhMesh {


  hhCompShader fieldShader;
  hhCompShader cubeShader = hhCompShader("marchingCubes/marchingVertices.comp");
  SSBO<gridPoint> gridSSBO;

  glm::vec3 gridMin;
  glm::vec3 gridMax;
  glm::vec3 gridRes;
  std::vector<gridPoint> fieldGrid;

  cubesMesh(
    glm::vec3 min,
    glm::vec3 max,
    glm::vec3 res,
    const char* shader) 
      : hhMesh(3), gridMin(min), gridMax(max), gridRes(res) {
    
    fieldShader.loadFile(shader);

    glm::vec3 step = (gridMax - gridMin) / (gridRes - glm::vec3(1.0));

    fieldGrid.clear();
    for (int ix = 0; ix < res.x; ix++) {
      for (int iy = 0; iy < res.y; iy++) {
        for (int iz = 0; iz < res.z; iz++) {

          gridPoint newPoint;
          newPoint.pos = gridMin + glm::vec3(
            ix * step.x,
            iy * step.y,
            iz * step.z
          );
          fieldGrid.push_back(newPoint);
        }
      }
    }
    gridSSBO.uploadData(fieldGrid, true);

    fieldShader.addSSBO(std::make_shared<SSBO<gridPoint>>(gridSSBO), 0);
    fieldShader.setUniform("t", 0.0f);

    cubeShader.addSSBO(std::make_shared<SSBO<gridPoint>>(gridSSBO), 0);

    fieldShader.dispatch(gridRes);
    cubeShader.dispatch(gridRes - glm::vec3(1.0));

    // write marchingVertices shader

  }

};



#endif