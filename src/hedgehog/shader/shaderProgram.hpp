#ifndef SHADER_PROGRAM
#define SHADER_PROGRAM

#include "../util/getTxt.hpp"
#include "../buffer/SSBO.hpp"
#include "templates.hpp"

GLuint compileShader(const char* source, GLenum shaderType) {
    GLint success;
    
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        glDeleteShader(shader);
        std::cerr << "ERROR: Vertex shader compilation failed\n" << infoLog << std::endl;
        return 0;
    }
    return shader;
}

struct ShaderSignature {
    // Shader stages
    bool usesVertexShader = true;
    bool usesFragmentShader = true;

    // Vertex layout
    bool usesPos = true;
    bool usesNorm = false;
    bool usesCol = false;
    bool usesUV = false;
    bool is3D = true;

    // Textures
    bool usesBaseColorTextureA = false;
    bool usesBaseColorTextureB = false;
    bool usesDepthMap = false;
    bool usesNormalMap = false;
    bool usesSpecularMap = false;
    bool usesEmissiveMap = false;

    bool usesCustomMaterial = false;

    // Lighting
    bool usesLighting = false;
    bool usesSpecular = false;
    bool usesPBR = false;

    // Geometry & pipeline features
    bool usesVertexColor = false;

    // Optional named features
    std::unordered_set<std::string> customBlocks;

    // For identification / hashing
    std::string uniqueID;

    bool operator==(const ShaderSignature& other) const {
    return usesVertexShader == other.usesVertexShader &&
           usesFragmentShader == other.usesFragmentShader &&
           usesBaseColorTextureA == other.usesBaseColorTextureA &&
           usesBaseColorTextureB == other.usesBaseColorTextureB &&
           usesDepthMap == other.usesDepthMap &&
           usesNormalMap == other.usesNormalMap &&
           usesSpecularMap == other.usesSpecularMap &&
           usesEmissiveMap == other.usesEmissiveMap &&
           usesLighting == other.usesLighting &&
           usesSpecular == other.usesSpecular &&
           usesPBR == other.usesPBR &&
           usesVertexColor == other.usesVertexColor &&
           customBlocks == other.customBlocks;
    }

    size_t hash() const {
        // Combine fields into a hash (can be optimized with boost or a custom function)
        size_t h = std::hash<bool>()(usesVertexShader) ^
                   std::hash<bool>()(usesFragmentShader) ^
                   std::hash<bool>()(usesBaseColorTextureA) ^
                   std::hash<bool>()(usesBaseColorTextureB) ^
                   std::hash<bool>()(usesDepthMap) ^
                   std::hash<bool>()(usesNormalMap) ^
                   std::hash<bool>()(usesSpecularMap) ^
                   std::hash<bool>()(usesEmissiveMap) ^
                   std::hash<bool>()(usesLighting) ^
                   std::hash<bool>()(usesSpecular) ^
                   std::hash<bool>()(usesPBR) ^
                   std::hash<bool>()(usesVertexColor);
        for (const auto& block : customBlocks) {
            h ^= std::hash<std::string>()(block);
        }
        return h;
    }
};
namespace std {
    template <>
    struct hash<ShaderSignature> {
        size_t operator()(const ShaderSignature& sign) const {
            return sign.hash(); // Calls your member function
        }
    };
}



class hhShaderProgram {
  private:
    GLuint ID;
  public:

    std::string vertSrc = "";
    std::string fragSrc = "";

    std::unordered_map<int, std::shared_ptr<baseSSBO>> SSBOs;
    void addSSBO(std::shared_ptr<baseSSBO> newStorageBuffer, unsigned int bindPoint);

    hhShaderProgram() {}
    hhShaderProgram(const char* vertShaderSrcFile, const char* fragShaderSrcFile);
    void genShader(const std::string& vertSrc, const std::string& fragSrc);
    hhShaderProgram(const std::string& vertSrc, const std::string& fragSrc) {
      genShader(vertSrc, fragSrc);
    };

    void use();
    GLuint getID() {return ID;};
    void destroy();
    ~hhShaderProgram() {
      if (ID != 0) {
        glDeleteProgram(ID);
        ID = 0;
      }
    }

    void buildFromSignature(const ShaderSignature& sign);
    void addVertBlock(const std::string& newBlock) {
      vertSrc += newBlock;
      vertSrc += "\n\n";
    }
    void addFragBlock(const std::string& newBlock) {
      fragSrc += newBlock;
      fragSrc += "\n\n";
    }

    void setUniform(const std::string &name, int value) {
      glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setUniform(const std::string &name, float value) {
      glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setUniform(const std::string &name, std::vector<float> value) {
      glUniform1fv(glGetUniformLocation(ID, name.c_str()), value.size(), value.data());
    }
    void setUniform(const std::string &name, const glm::vec2 &value) {
      glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setUniform(const std::string &name, const glm::vec3 &value) {
      glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setUniform(const std::string &name, const glm::vec4 &value) {
      glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setUniform(const std::string &name, const glm::mat4 &value) {
      glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }
    void setUniform(const std::string &name, const hhTex& value) {
      glUniform1i(glGetUniformLocation(ID, name.c_str()), value.getID());
    }

};

hhShaderProgram::hhShaderProgram(const char* vertShaderSrcFile, const char* fragShaderSrcFile) {

    std::string vertFilePath = "../src/shaders/" + std::string(vertShaderSrcFile);
    std::string fragFilePath = "../src/shaders/" + std::string(fragShaderSrcFile);

    std::string vertSrc = getFileContents(vertFilePath.c_str());
    std::string fragSrc = getFileContents(fragFilePath.c_str());
    
    genShader(vertSrc, fragSrc);
    
}

void hhShaderProgram::genShader(const std::string& vertSrc, const std::string& fragSrc) {
  
  GLuint vertShader = compileShader(vertSrc.c_str(), GL_VERTEX_SHADER);
  GLuint fragShader = compileShader(fragSrc.c_str(), GL_FRAGMENT_SHADER);

  if (vertShader == 0 || fragShader == 0) {
      std::cerr << "Failed to compile one or more shaders. Program creation aborted." << std::endl;
      return;
  }

  GLint success;
  GLchar infoLog[512];

  ID = glCreateProgram();
  glAttachShader(ID, vertShader);
  glAttachShader(ID, fragShader);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      std::cerr << "ERROR: Shader program linking failed\n" << infoLog << std::endl;
  }

  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
}

void hhShaderProgram::use() {
  glUseProgram(ID);
  for (const auto& pair : SSBOs) {
    pair.second->bind(pair.first);
  }
}

void hhShaderProgram::destroy() {
  if (ID != 0) {
    glDeleteProgram(ID);
    ID = 0;
  }
}

void hhShaderProgram::addSSBO(std::shared_ptr<baseSSBO> newBuffer, unsigned int bindPoint) {
  
  for (const auto& pair: SSBOs) {
    if (bindPoint == pair.first) {
      std::cout << "SSBO bind point already in use" << std::endl;
      return;
    }
  }

  SSBOs[bindPoint] = newBuffer;
}


void hhShaderProgram::buildFromSignature(const ShaderSignature& sign) {

  // uniforms and general setup

  addVertBlock(shaderBlocks::setup::version);
  addFragBlock(shaderBlocks::setup::version);
  
  if (sign.usesPos) { 
    addVertBlock(shaderBlocks::setup::vertInPos);
    addVertBlock(shaderBlocks::setup::vertOutPos);
    addFragBlock(shaderBlocks::setup::fragInPos);
  }
  if (sign.usesNorm) { 
    addVertBlock(shaderBlocks::setup::vertInNorm);
    addVertBlock(shaderBlocks::setup::vertOutNorm);
    addFragBlock(shaderBlocks::setup::fragInNorm);
  }
  if (sign.usesCol) { 
    addVertBlock(shaderBlocks::setup::vertInCol);
    addVertBlock(shaderBlocks::setup::vertOutCol);
    addFragBlock(shaderBlocks::setup::fragInCol);
  }
  if (sign.usesUV) { 
    addVertBlock(shaderBlocks::setup::vertInTex);
    addVertBlock(shaderBlocks::setup::vertOutTex);
    addFragBlock(shaderBlocks::setup::fragInTex);
  }

  addFragBlock(shaderBlocks::setup::fragOutCol);

  addVertBlock(shaderBlocks::setup::sceneUBO);
  addFragBlock(shaderBlocks::setup::sceneUBO);

  if (sign.usesCustomMaterial) {
    addFragBlock(shaderBlocks::setup::materialUBO);
  }
  if (sign.usesBaseColorTextureA) {
    addFragBlock(shaderBlocks::setup::useColorTexA);
  }
  if (sign.usesBaseColorTextureB) {
    addFragBlock(shaderBlocks::setup::useColorTexB);
  }
  if (sign.usesDepthMap) {
    addFragBlock(shaderBlocks::setup::useDepthMap);
  }
  if (sign.usesSpecularMap) {
    addFragBlock(shaderBlocks::setup::useSpecularMap);
  }
  if (sign.usesNormalMap) {
    addFragBlock(shaderBlocks::setup::useNormalMap);
  }

  // vert shader body

  if (sign.is3D) {
    addVertBlock(shaderBlocks::vertShaderBody::basic3DVertMain);
    if (sign.usesUV) {
      addVertBlock(shaderBlocks::vertShaderBody::setVertTex);
    }
    addVertBlock(shaderBlocks::vertShaderBody::glPos3D);
  } else {
    addVertBlock(shaderBlocks::vertShaderBody::basic2DVertMain);
    if (sign.usesUV) {
      addVertBlock(shaderBlocks::vertShaderBody::setVertTex);
    }
    addVertBlock(shaderBlocks::vertShaderBody::glPos2D);
  }

  // frag shader body
}

#endif

