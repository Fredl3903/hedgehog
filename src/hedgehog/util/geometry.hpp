#ifndef HH_HASH
#define HH_HASH

#include <C:/OpenGL/glm/glm/glm.hpp>
#include "../model/mesh.hpp"
#include <functional>

struct hhVert {
  glm::vec3 pos;
  float _pad0;

  glm::vec3 norm = glm::vec3(0.0f);
  float _pad1;

  glm::vec3 col = glm::vec3(0.0f);
  float _pad2;

  glm::vec2 uv = glm::vec2(0.0f);

  int update = 0;
  int posID = 0;

  bool posEqual(const hhVert& other) const {
    return pos == other.pos;
  };
  bool operator==(const hhVert& other) const {
    return pos == other.pos &&
      norm == other.norm &&
      col == other.col &&
      uv == other.uv;
  };
};
struct hhEdge {
  glm::vec3 A, B;
  unsigned int ia, ib;
};
struct hhTri {
  glm::vec3 A = glm::vec3(0.0);
  float _padA;
  glm::vec3 B = glm::vec3(0.0);
  float _padB;
  glm::vec3 C = glm::vec3(0.0);
  float _padC;
  glm::vec3 centroid = glm::vec3(0.0);
  float _padCentroid;
};
struct hhFace {
  std::vector<hhVert> corners;
};

// Hash function for glm::vec3 to enable position ID mapping
struct Vec3Hash {
    std::size_t operator()(const glm::vec3& v) const {
        std::size_t h1 = std::hash<float>{}(v.x);
        std::size_t h2 = std::hash<float>{}(v.y);
        std::size_t h3 = std::hash<float>{}(v.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
// Hash function for hhVert to enable unordered_map usage
struct hhVertHash {
  std::size_t operator()(const hhVert& v) const {
    std::size_t h1 = std::hash<float>{}(v.pos.x);
    std::size_t h2 = std::hash<float>{}(v.pos.y);
    std::size_t h3 = std::hash<float>{}(v.pos.z);
    std::size_t h4 = std::hash<float>{}(v.norm.x);
    std::size_t h5 = std::hash<float>{}(v.norm.y);
    std::size_t h6 = std::hash<float>{}(v.norm.z);
    std::size_t h7 = std::hash<float>{}(v.col.x);
    std::size_t h8 = std::hash<float>{}(v.col.y);
    std::size_t h9 = std::hash<float>{}(v.col.z);
    std::size_t h10 = std::hash<float>{}(v.uv.x);
    std::size_t h11 = std::hash<float>{}(v.uv.y);
    
    return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5) ^ 
            (h7 << 6) ^ (h8 << 7) ^ (h9 << 8) ^ (h10 << 9) ^ (h11 << 10);
  }
};

hhVert createVertex(const std::vector<int>& vI, 
                    const std::vector<glm::vec3>& positions,
                    const std::vector<glm::vec3>& normals,
                    const std::vector<glm::vec3>& colors,
                    const std::vector<glm::vec2>& texCoords) {
  hhVert newVertex;
    
  // Position is always first index
  if (!vI.empty() && vI[0] < positions.size()) {
    newVertex.pos = positions[vI[0]];
    newVertex.col = colors[vI[0]];
  }
  
  // Handle texture coordinates (second index if present)
  if (vI.size() > 1 && vI[1] >= 0 && vI[1] < texCoords.size()) {
    newVertex.uv = texCoords[vI[1]];
  } else if (texCoords.size() == positions.size() && vI[0] < texCoords.size()) {
    newVertex.uv = texCoords[vI[0]];
  }
  
  // Handle normals (third index if present, or second if no tex coords)
  if (vI.size() > 2 && vI[2] >= 0 && vI[2] < normals.size()) {
    newVertex.norm = normals[vI[2]];
  } else if (vI.size() == 2 && vI[1] >= 0 && vI[1] < normals.size()) {
    newVertex.norm = normals[vI[1]];
  } else if (normals.size() == positions.size() && vI[0] < normals.size()) {
    newVertex.norm = normals[vI[0]];
  }
  
  return newVertex;
}

// Helper function to parse face vertex string
std::vector<int> parseFaceVertex(const std::string& vertStr) {
  std::vector<int> indices;
  std::string temp = vertStr + "/";
  int last = 0;
    
  for (int i = 0; i < temp.length(); i++) {
    if (temp[i] == '/') {
      if (i > last) {
        indices.push_back(std::stoi(temp.substr(last, i - last)) - 1);
      } else {
        indices.push_back(-1); // Empty index (e.g., "1//1")
      }
      last = i + 1;
    }
  }
    
  return indices;
}


#endif