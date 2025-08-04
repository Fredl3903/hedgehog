#ifndef MESH_3D
#define MESH_3D

#include "mesh.hpp"

class hhMesh3d : public hhMesh {

  protected:

    void makeAbstract() const override {};

  public:

    hhMesh3d() : hhMesh(3) {}
    hhMesh3d(std::vector<hhVert>&& data, std::vector<unsigned int> indices) : hhMesh(3) {
      geometry(data, indices);
    };
    hhMesh3d(const std::string& filename) : hhMesh(3) {
      geometry(filename);
    }
    using hhMesh::geometry;
    void geometry(const std::string& filename);

    void reverseNorms() {
      for (hhVert& v : vertices) {
        v.norm = -v.norm;
      }
    }

    void translate(glm::vec3 v) {
      for (hhVert& vert : vertices) {
        vert.pos += v;
      }
    }
    void scale(glm::vec3 centre, glm::vec3 scaleFactor) {
      if (dimensionality != 3) {
        std::cout << "wrong dimensionality in hhMesh::scale call" << std::endl;
        return;
      }
      
      for (hhVert& vert : vertices) {
        vert.pos = centre + scaleFactor * (vert.pos - centre);
      }
    }

    void computeFaceNorms(unsigned int style); // 0 - vertex normals; 1 - surface normals
};

void hhMesh3d::geometry(const std::string& filename) {

  std::cout << "Opening " << filename << " ..." << std::endl;
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Could not open the file.\n";
    return;
  }

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec3> colors;
  std::vector<glm::vec2> texCoords;

  // Use hash maps for efficient duplicate detection
  std::unordered_map<hhVert, int, hhVertHash> vertexMap;
  std::unordered_map<glm::vec3, int, Vec3Hash> positionIDMap;
  int nextPosID = 0;

  // Pre-allocate vectors for better performance
  positions.reserve(10000);
  normals.reserve(10000);
  colors.reserve(10000);
  texCoords.reserve(10000);
  vertices.reserve(10000);
  indices.reserve(30000);

  std::string line;
  int lineCount = 0;
  int faceCount = 0;

  std::cout << "Parsing OBJ file..." << std::endl;

  while (std::getline(file, line)) {
    lineCount++;
    
    // Progress tracking for large files
    if (lineCount % 10000 == 0) {
      std::cout << "Processed " << lineCount << " lines, " << faceCount << " faces, " 
                << vertices.size() << " unique vertices" << std::endl;
    }
    
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;
    
    if (prefix == "v") {
      glm::vec3 v;
      iss >> v.x >> v.y >> v.z;
      positions.push_back(v);
      colors.push_back(glm::vec3(0.7f)); // Default color
    } 
    else if (prefix == "vn") {
      glm::vec3 n;
      iss >> n.x >> n.y >> n.z;
      normals.push_back(n);
    } 
    else if (prefix == "vt") {
      glm::vec2 t;
      iss >> t.x >> t.y;
      texCoords.push_back(t);
    } 
    else if (prefix == "f") {
      faceCount++;
      
      std::vector<std::string> faceVertices;
      std::string vertStr;
      
      // Read all vertices for this face
      while (iss >> vertStr) {
        faceVertices.push_back(vertStr);
      }
      
      if (faceVertices.empty()) continue;
      
      // Process face vertices
      std::vector<unsigned int> faceIndices;
      faceIndices.reserve(faceVertices.size());
      
      for (const std::string& vStr : faceVertices) {
        std::vector<int> vI = parseFaceVertex(vStr);
        hhVert newVertex = createVertex(vI, positions, normals, colors, texCoords);
        
        // Set position ID
        auto posIt = positionIDMap.find(newVertex.pos);
        if (posIt != positionIDMap.end()) {
          newVertex.posID = posIt->second;
        } else {
          newVertex.posID = nextPosID++;
          positionIDMap[newVertex.pos] = newVertex.posID;
        }
        
        // Check for duplicate vertex
        auto vertIt = vertexMap.find(newVertex);
        if (vertIt != vertexMap.end()) {
          faceIndices.push_back(vertIt->second);
        } else {
          int newIndex = vertices.size();
          vertices.push_back(newVertex);
          vertexMap[newVertex] = newIndex;
          faceIndices.push_back(newIndex);
        }
      }
      
      // Triangulate face if necessary
      if (faceIndices.size() == 3) {
        // Triangle - add directly
        indices.insert(indices.end(), faceIndices.begin(), faceIndices.end());
      } else if (faceIndices.size() == 4) {
        // Quad - triangulate
        indices.insert(indices.end(), {
          faceIndices[0], faceIndices[1], faceIndices[2],
          faceIndices[2], faceIndices[3], faceIndices[0]
        });
      } else if (faceIndices.size() > 4) {
        // N-gon - fan triangulation
        for (int i = 1; i < faceIndices.size() - 1; i++) {
          indices.insert(indices.end(), {
            faceIndices[0], faceIndices[i], faceIndices[i + 1]
          });
        }
      }
    }
  }

  std::cout << "OBJ parsing complete!" << std::endl;
  std::cout << "Final stats: " << positions.size() << " positions, " 
            << vertices.size() << " unique vertices, " 
            << indices.size() / 3 << " triangles" << std::endl;

}


#endif