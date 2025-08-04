#ifndef UNIFORM_BUFFER
#define UNIFORM_BUFFER

#include <C:/OpenGL/GLFW//include/GLFW/glfw3.h>
#include <C:/OpenGL/GLEW/include/GL/glew.h>

class baseUBO {
  protected:
    GLuint bufferID = 0;
    GLuint bindingPoint = 0;
    GLsizeiptr size = 0;

  public:
    baseUBO(GLuint bindingPoint, GLsizeiptr size)
      : bindingPoint(bindingPoint), size(size) {
      glGenBuffers(1, &bufferID);
      glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
      glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
      glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, bufferID);
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    virtual ~baseUBO() {
      glDeleteBuffers(1, &bufferID);
    }

    void bind() const {
      glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
    }

    void unbind() const {
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
};

template<typename T>
class hhUBO : public baseUBO {
  public:
    hhUBO(GLuint bindingPoint) : baseUBO(bindingPoint, sizeof(T)) {}

    void setData(const T& data) {
      bind();
      glBufferSubData(GL_UNIFORM_BUFFER, 0, size, &data);
      unbind();
    }
};

#endif