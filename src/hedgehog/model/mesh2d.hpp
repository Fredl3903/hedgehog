#ifndef MESH_2D
#define MESH_2D

#include "mesh.hpp"

class hhMesh2d : public hhMesh {

  protected:

    void makeAbstract() const override {};

  public:

    hhMesh2d() : hhMesh(2) {}
    hhMesh2d(std::vector<hhVert>&& data, std::vector<unsigned int> indices) : hhMesh(2) {
      geometry(data, indices);
    };

    void translate(glm::vec2 v) {
      for (hhVert& vert : vertices) {
        vert.pos += glm::vec3(v, 0.0);
      }
    }
    void scale(glm::vec2 centre, glm::vec2 scaleFactor) {      
      for (hhVert& vert : vertices) {
        glm::vec3 c = glm::vec3(centre.x, centre.y, 0.0);
        vert.pos = c + glm::vec3(scaleFactor, 0.0) * (vert.pos - c);
      }
    }
};

#endif