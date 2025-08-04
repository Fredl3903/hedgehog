#ifndef FBO
#define FBO

#include "renderTarget.hpp"
#include "../material/fboDepthTex.hpp"
#include "../layer/special/screenQuad.hpp"

class hhFBO : public hhRenderTarget {

public:

  GLuint framebuffer;
  
  std::shared_ptr<hhImgTex> colorTex;
  std::shared_ptr<hhFBODepthTex> depthTex;
  
  hhFBO(int width, int height) : 
    hhRenderTarget(width, height) {

    colorTex = std::make_shared<hhImgTex>(width, height);
    depthTex = std::make_shared<hhFBODepthTex>(width, height);
    
    setupFramebuffer();
  }
  ~hhFBO() {
    cleanup();
  }

  void setupQuad();

  void resetTextures() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex->getID(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex->getID(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void hedge() override;
  void hog() override;

private:
  void setupFramebuffer();
  void cleanup();
};

void hhFBO::hedge() {
  // Bind framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glViewport(0, 0, width, height);
  glEnable(GL_DEPTH_TEST);
  
  // Clear
  glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void hhFBO::hog() {
  // Unbind framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void hhFBO::setupFramebuffer() {
  // Generate framebuffer
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  // Create color texture
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex->getID(), 0);

  // Create depth texture
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex->getID(), 0);

  // Check framebuffer completeness
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      std::cout << "Framebuffer not complete!" << std::endl;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// void hhFBO::setupQuad() {
//   std::shared_ptr<screenQuadLayer> quadLayer = std::make_shared<screenQuadLayer>();
//   std::shared_ptr<hhScene> quadScene = std::make_shared<hhScene>();
//   quadScene->addLayer(quadLayer);
//   addScene(quadScene);
// }

void hhFBO::cleanup() {
  if (framebuffer) glDeleteFramebuffers(1, &framebuffer);
}


#endif