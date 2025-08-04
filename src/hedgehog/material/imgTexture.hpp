#ifndef IMG_TEX
#define IMG_TEX

#include <iostream>
#include "tex2d.hpp"

class hhImgTex : public hhTex2d {

  public:
    hhImgTex(int w, int h) : hhTex2d(w, h, GL_RGBA, GL_RGBA, GL_FLOAT) {
      std::vector<pixel> data(w * h);
      for (pixel& p : data) {
        p.r = 1.0;
        p.g = 1.0;
        p.b = 1.0;
        p.a = 1.0;
      }
      uploadData(data, w, h);
    }
    hhImgTex(int w, int h, float alpha) : hhTex2d(w, h, GL_RGBA, GL_RGBA, GL_FLOAT) {
      std::vector<pixel> data(w * h);
      for (pixel& p : data) {
        p.r = 1.0;
        p.g = 1.0;
        p.b = 1.0;
        p.a = alpha;
      }
      uploadData(data, w, h);
    }
    hhImgTex(const std::string& filename) : hhTex2d(1, 1) {
      uploadData(filename);
    }

    using hhTex2d::uploadData;
    void uploadData(const std::string& filename);

};

class hhImgTexManual : public hhTex2d {

  public:
  hhImgTexManual(int w, int h) : hhTex2d(w, h, GL_RGBA, GL_RGBA, GL_FLOAT) {
    std::vector<pixel> data(w * h);
    for (pixel& p : data) {
      p.r = 1.0;
      p.g = 1.0;
      p.b = 1.0;
      p.a = 1.0;
    }
    uploadData(data, w, h);
  }

};

void hhImgTex::uploadData(const std::string& filename) {
  
  std::string filePath = "../src/img/" + std::string(filename);
  int w, h, channels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(filePath.c_str(), &w, &h, &channels, 0);
  printf("Loaded image: %s | size: %d x %d | channels: %d\n", filePath.c_str(), w, h, channels);
  if (data) {
    internalFormat = channels == 4 ? GL_RGBA : GL_RGB;
    format = channels == 4 ? GL_RGBA : GL_RGB;
    width = w;
    height = h;
    bind();
    glTexImage2D(
      target, 0, internalFormat,
      width, height, 0, format, type, data
    );
    // glGenerateMipmap(target);
    unbind();
  } else {
      printf("Failed to load image: %s\n", stbi_failure_reason());
  }
  stbi_image_free(data);
}

#endif