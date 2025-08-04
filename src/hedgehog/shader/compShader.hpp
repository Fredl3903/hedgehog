#ifndef COMP_SHADER
#define COMP_SHADER

#include "../buffer/SSBO.hpp"
#include <functional>
#include <memory>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

std::string getCompContents(const char* filePath) {
    
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return nullptr;
    }
    
    std::string contentsStr;
    std::string line;
    while (getline(file, line)) {
        contentsStr += line;
        contentsStr += "\n";
    }

    // Close the file
    file.close();

    // std::cout << "___________________________________" << std::endl;
    // std::cout << contentsStr << std::endl;
    // std::cout << "___________________________________" << std::endl;

    return contentsStr;
}

GLuint compileCompShader(const char* source, GLenum shaderType) {
  GLint success;
  
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    glDeleteShader(shader);
    std::cerr << "ERROR: Comp shader compilation failed\n" << infoLog << std::endl;
    return 0;
  }
  return shader;
}

class hhCompShader {

  private:

    GLuint ID;

  public:
    
    std::unordered_map<int, std::shared_ptr<baseSSBO>> SSBOs;

    hhCompShader() {}
    hhCompShader(const char* compShaderSrcFile) {
      loadFile(compShaderSrcFile);
    }

    void loadFile(const char* compShaderSrcFile);
    
    void addSSBO(std::shared_ptr<baseSSBO> newStorageBuffer, unsigned int bindPoint);
    void use() {glUseProgram(ID); };
    void unuse() { glUseProgram(0); };
    GLuint getID() { return ID; }
    void dispatch(glm::vec3 groups);

    void setUniform(const std::string &name, int value) {
      glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setUniform(const std::string &name, float value) {
      glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
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


    ~hhCompShader() {
      if (ID != 0) {
        glDeleteProgram(ID);
        ID = 0;
      }
    }
};

void hhCompShader::loadFile(const char* compShaderSrcFile) {
  
  std::string filePath = "../src/shaders/computeShaders/" + std::string(compShaderSrcFile);
  std::string src = getCompContents(filePath.c_str());

  GLuint compShader = compileCompShader(src.c_str(), GL_COMPUTE_SHADER);

  if (compShader == 0) {
    std::cerr << "Failed to compile shader. Program creation aborted." << std::endl;
    return;
  }

  GLint success;
  GLchar infoLog[512];
  ID = glCreateProgram();
  if (ID == 0) {
    std::cout << "glCreateProgram() failed in hhCompShader" << std::endl;
    return;
  }
  glAttachShader(ID, compShader);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cerr << "ERROR: Shader program linking failed\n" << infoLog << std::endl;
  }
  glDeleteShader(compShader);
}

void hhCompShader::addSSBO(std::shared_ptr<baseSSBO> newBuffer, unsigned int bindPoint) {
  
  for (const auto& pair: SSBOs) {
    if (bindPoint == pair.first) {
      std::cout << "SSBO bind point already in use" << std::endl;
      return;
    }
  }

  SSBOs[bindPoint] = newBuffer;
}
void hhCompShader::dispatch(glm::vec3 groups) { // simple auto setup
  if (groups == glm::vec3(0)) {
    std::cout << "dispatch called with 0 workgroups" << std::endl;
    return;
  }

  glUseProgram(ID); 

  for (const auto& pair : SSBOs) {
    pair.second->bind(pair.first);
  }
  
  glDispatchCompute(groups.x, groups.y, groups.z);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
  glUseProgram(0);
}

#endif