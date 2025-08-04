#ifndef FBO_DEPTH_TEX
#define FBO_DEPTH_TEX

#include "tex2d.hpp"

class hhFBODepthTex : public hhTex2d {

  public:

    hhFBODepthTex(int w, int h) : hhTex2d(w, h, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT) {}

};

#endif