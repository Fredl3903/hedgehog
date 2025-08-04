#ifndef BASIC_LAYER
#define BASIC_LAYER

#include "layer.hpp"
#include "special/screenQuad.hpp"
#include "../model/model3d.hpp"


class basic3dLayer : public hhLayer {
  
  public:

    basic3dLayer() : hhLayer() {};    
    basic3dLayer(int zIndex) : hhLayer(3, zIndex) {
      setLayerShaders(true);
    }

    void enableLighting() { setLayerShaders(true); }
    void disableLighting() { setLayerShaders(false); }

    void uploadOBJ(const std::string& filename);

    virtual void renderLayer() override;

  private:
    void setLayerShaders(const std::shared_ptr<hhShaderProgram>& newShaderProgram) override {}
    void setLayerShaders(bool lighting) {
      if (lighting) {
        std::shared_ptr<hhShaderProgram> basicShaders = std::make_shared<hhShaderProgram>("basic3d.vert","basic3dLit.frag");
        layerShaders = basicShaders;
      } else {
        std::shared_ptr<hhShaderProgram> basicShaders = std::make_shared<hhShaderProgram>("basic3d.vert","basic3dUnlit.frag");
        layerShaders = basicShaders;
      }
    };
};

void basic3dLayer::renderLayer() {  

  // Check shaders and camera are valid before proceeding

  if (!layerShaders) {
    std::cout << "null ptr to shaders in renderLayer()" << std::endl;
    return; // Exit early if shader is null
  } else {
    layerShaders->use();
  }

  if (!layerCamera) {
    std::cout << layerCamera << " - null ptr to camera in renderLayer()" << std::endl;
    return; // Exit early if camera is null
  } else {
    layerCamera->update();
  }

  setProjUniforms();
  setCamUniforms();
  renderAllModels();
};


void basic3dLayer::uploadOBJ(const std::string& filename) {

  std::cout << "Opening " << filename << " ..." << std::endl;
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Could not open the file.\n";
    return;
  }

  std::vector<hhVert> vertices;
  std::vector<unsigned int> indices;
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec3> colors;
  std::vector<glm::vec2> texCoords;

  // Use hash maps for efficient duplicate detection
  std::unordered_map<hhVert, int, hhVertHash> vertexMap;
  std::unordered_map<glm::vec3, int, Vec3Hash> positionIDMap;
  int nextPosID = 0;

  bool firstModel = true;
  int objectIndexOffset = 0;

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
    if (lineCount % 100000 == 0) {
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
          int newIndex = vertices.size() - objectIndexOffset;
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
    } else if (prefix == "o") {
      if (vertices.size() > 0) {
        std::shared_ptr<hhMesh3d> newMesh = std::make_shared<hhMesh3d>(std::move(vertices), std::move(indices));
        std::shared_ptr<hhModel3d> newModel = std::make_shared<hhModel3d>(GL_TRIANGLES);

        newModel->addMesh(newMesh);
        addModel(newModel);
        newModel->setBuffers();

        vertices.clear();
        indices.clear();
        objectIndexOffset += vertices.size();
      }
    }
  }
  if (vertices.size() > 0) {
    std::shared_ptr<hhMesh3d> newMesh = std::make_shared<hhMesh3d>(std::move(vertices), std::move(indices));
    std::shared_ptr<hhModel3d> newModel = std::make_shared<hhModel3d>(GL_TRIANGLES);

    newModel->addMesh(newMesh);
    addModel(newModel);
    newModel->setBuffers();

    vertices.clear();
    indices.clear();
    objectIndexOffset += vertices.size();
  }  

}

#endif