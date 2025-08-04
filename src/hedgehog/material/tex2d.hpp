#ifndef TEX_2D
#define TEX_2D

#include "tex.hpp"

class hhTex2d : public hhTex {
  public:

    hhTex2d(int w, int h, 
    GLenum internalFormat = GL_RGBA,
    GLenum format = GL_RGBA,
    GLenum type = GL_UNSIGNED_BYTE);
    hhTex2d(const hhTex2d& other);

    void copyFrom(const hhTex2d& other);
    
    void uploadData(const void* data, int w, int h) override;
    void uploadData(std::vector<pixel>& data, int w, int h);
    void updateData(const void* data, int xoffset, int yoffset, int w, int h) override;
    void updateData(std::vector<pixel>& data, int xoffset, int yoffset, int w, int h);
};

hhTex2d::hhTex2d(int w, int h, 
  GLenum internalFormat,
  GLenum format,
  GLenum type) : hhTex(w, h, GL_TEXTURE_2D, internalFormat, format, type) {

  bind();
  glTexImage2D(target, 0, internalFormat, width, height,
  0, format, type, nullptr);
  setParameters();
  unbind();
}

hhTex2d::hhTex2d(const hhTex2d& other)
  : hhTex(other.width, other.height, other.target,
          other.internalFormat, other.format, other.type) {

  // Create new texture and copy parameters
  glGenTextures(1, &ID);
  bind();

  // Allocate memory
  glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, nullptr);

  // Temporary buffer to store pixel data
  size_t bytesPerPixel = 4 * sizeof(float); // Assuming GL_RGBA + GL_FLOAT
  std::vector<unsigned char> pixels(width * height * bytesPerPixel);

  // Bind source texture
  glBindTexture(other.target, other.ID);
  glGetTexImage(other.target, 0, other.format, other.type, pixels.data());

  // Rebind destination and upload data
  glBindTexture(target, ID);
  glTexSubImage2D(target, 0, 0, 0, width, height, format, type, pixels.data());

  setParameters();
  unbind();
}

void hhTex2d::copyFrom(const hhTex2d& other) {
  // Reallocate if dimensions or format changed
  if (width != other.width || height != other.height ||
      internalFormat != other.internalFormat ||
      format != other.format || type != other.type) {
    
    width = other.width;
    height = other.height;
    internalFormat = other.internalFormat;
    format = other.format;
    type = other.type;

    bind();
    glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, nullptr);
    setParameters();
    unbind();
  }

  // Estimate byte size per pixel (minimal safe fallback)
  GLint bpp = 4; // GL_RGBA
  GLint bytesPerComponent = 1;

  if (type == GL_FLOAT) bytesPerComponent = 4;
  else if (type == GL_UNSIGNED_BYTE) bytesPerComponent = 1;
  else if (type == GL_HALF_FLOAT) bytesPerComponent = 2;
  // ... Add more as needed

  size_t bytesPerPixel = bpp * bytesPerComponent;
  std::vector<unsigned char> pixels(width * height * bytesPerPixel);

  // Read data from source
  glBindTexture(other.target, other.ID);
  glGetTexImage(other.target, 0, other.format, other.type, pixels.data());

  // Upload to this texture
  bind();
  glTexSubImage2D(target, 0, 0, 0, width, height, format, type, pixels.data());
  unbind();
}

void hhTex2d::uploadData(std::vector<pixel>& data, int w, int h) {
  width = w;
  height = h;
  bind();
  glTexImage2D(
    target, 0, internalFormat,
    width, height, 0, format, type, data.data()
  );
  unbind();
}
void hhTex2d::uploadData(const void* data, int w, int h) {
  width = w;
  height = h;
  bind();
  glTexImage2D(
    target, 0, internalFormat,
    width, height, 0, format, type, data
  );
  unbind();
}

void hhTex2d::updateData(std::vector<pixel>& data, int xoffset, int yoffset, int w, int h) {
  bind();
  glTexSubImage2D(
    target, 0, xoffset, yoffset,
    w, h, format, type, data.data()
  );
  unbind();
}
void hhTex2d::updateData(const void* data, int xoffset, int yoffset, int w, int h) {
  bind();
  glTexSubImage2D(
    target, 0, xoffset, yoffset,
    w, h, format, type, data
  );
  unbind();
}


#endif
