#ifndef HH_TEXTURE
#define HH_TEXTURE

#include <C:/OpenGL/GLEW/include/GL/glew.h>
#include <C:/OpenGL/GLFW/include/GLFW/glfw3.h>
#include <C:/OpenGL/glm/glm/glm.hpp>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "../util/stb_image.h"

struct pixel {
  float r;
  float g;
  float b;
  float a;
};

class hhTex {

  public:
    // default values are for basic 2d img texture
    GLuint ID = 0;
    GLenum target;
    GLint internalFormat;
    GLenum format;
    GLenum type;
    int width = 0;
    int height = 0;
    int depth = 1;

    hhTex(int w, int h, 
    GLenum target, GLint internalFormat, 
    GLenum format, GLenum type)
    : width(w), height(h),
      target(target), internalFormat(internalFormat), 
      format(format), type(type) {
      glGenTextures(1, &ID);
    }
    virtual ~hhTex() {
      glDeleteTextures(1, &ID);
    }

    hhTex(hhTex&& other) noexcept;
    hhTex& operator=(hhTex&& other) noexcept;

    void bind(GLuint unit = 0) const;
    void unbind() const;

    virtual void setParameters();
    virtual void uploadData(const void* data, int w, int h) = 0;
    virtual void updateData(const void* data, int xoffset, int yoffset, int w, int h) = 0;

    GLuint getID() const { return ID; }
    GLenum getTarget() const {return target; }

};

void hhTex::bind(GLuint unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(target, ID);
}
void hhTex::unbind() const {
  glBindTexture(target, 0);
}

void hhTex::setParameters() {
  bind();
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
  unbind();
}

#endif