#ifndef STORAGE_BUFFER
#define STORAGE_BUFFER

#include <C:/OpenGL/GLEW/include/GL/glew.h>
#include <C:/OpenGL/GLFW/include/GLFW/glfw3.h>
#include <C:/OpenGL/glm/glm/glm.hpp>
#include <vector>


class baseSSBO {

  public:

    GLuint ID;

    int dataLength;
    int dataSize;
    virtual void bind(GLuint bindPoint) const = 0;
    GLuint getID() const { return ID; };

    virtual ~baseSSBO() {
      glDeleteBuffers(1, &ID);
    }

};

template<typename T>
class SSBO : public baseSSBO {
  public:
    std::vector<T> data;

    SSBO<T>() {}
    SSBO<T>(std::vector<T>& data, bool useRef) {
      uploadData(data, useRef);
    }
    SSBO<T>(std::vector<T>&& newData) {
      uploadData(data);
    }
    
    void uploadData(std::vector<T>& data, bool useRef);
    void uploadData(std::vector<T>&& data);
    
    void bind(GLuint bindPoint) const override;
    void update();

    std::vector<T> retrieve();
};

template<typename T>
void SSBO<T>::uploadData(std::vector<T>&& newData) {

  data = newData;

  dataLength = data.size();
  dataSize = data.size() * sizeof(T);
  std::cout << "dataSize: " << dataSize << std::endl;

  glGenBuffers(1, &ID);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
  glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, data.data(), GL_DYNAMIC_COPY);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

template<typename T>
void SSBO<T>::uploadData(std::vector<T>& data, bool useRef) {
  dataLength = data.size();
  dataSize = dataLength * sizeof(T);

  glGenBuffers(1, &ID);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
  glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, data.data(), GL_DYNAMIC_COPY);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

template<typename T>
void SSBO<T>::bind(GLuint bindPoint) const {
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindPoint, ID);
}

template<typename T>
void SSBO<T>::update() {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
  glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, dataSize, data.data());
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

template<typename T>
std::vector<T> SSBO<T>::retrieve() {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ID); 

  std::vector<T> outData(dataLength);
  glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, dataSize, outData.data());
  
  return outData;
}




template<typename T>
std::shared_ptr<SSBO<T>> getSSBOAs(std::shared_ptr<baseSSBO> base) {
    return std::dynamic_pointer_cast<SSBO<T>>(base);
}
template<typename T>
std::vector<T> retrieveFromSSBO(std::shared_ptr<baseSSBO> base) {
    auto casted = std::dynamic_pointer_cast<SSBO<T>>(base);
    if (!casted) {
        throw std::runtime_error("Failed to cast SSBO to expected type.");
    }
    return casted->retrieve();
}

#endif