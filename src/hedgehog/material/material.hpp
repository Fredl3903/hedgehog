#ifndef MATERIAL
#define MATERIAL

#include <C:/OpenGL/GLEW/include/GL/glew.h>
#include <C:/OpenGL/GLFW/include/GLFW/glfw3.h>
#include <C:/OpenGL/glm/glm/glm.hpp>

#include <memory>

#include "tex2d.hpp"
#include "imgTexture.hpp"
#include "fboDepthTex.hpp"
#include "normalMap.hpp"
#include "specularMap.hpp"

class hhMaterial {
  private:
    
    // properties

    float roughness;
    float metallic;

    glm::vec3 emissiveCol = glm::vec3(0.0);

    float opacity;
    float refractiveIndex; // default to vacuum

    std::shared_ptr<hhTex2d> colorTex1;
    std::shared_ptr<hhTex2d> colorTex2;
    std::shared_ptr<hhTex2d> normalMap;
    std::shared_ptr<hhTex2d> depthMap;
    std::shared_ptr<hhTex2d> specularMap;

    
  public:

    hhMaterial() { 
      // blank white textures
      std::shared_ptr<hhImgTex> defaultTex = std::make_shared<hhImgTex>(1, 1);
      std::shared_ptr<hhFBODepthTex> defaultDepth = std::make_shared<hhFBODepthTex>(1, 1);
      setSurface(0.5, 0.5);
      setEmission(glm::vec3(0.0));
      setTransparency(1.0, 1.0);
      setTexture1(defaultTex);
      setTexture2(defaultTex);
      setDepthMap(defaultDepth);
    }
    hhMaterial(hhMaterial&& other); // move constructor

    //Setters
    void setSurface(float rough, float metal) {
      roughness = rough;
      metallic = metal;
    }
    void setEmission(glm::vec3 emission) { emissiveCol = emission; }
    void setTransparency(float opaque, float rI) {
      opacity = opaque;
      refractiveIndex = rI;
    }
    void setTexture1(std::shared_ptr<hhTex2d> newTex) { colorTex1 = newTex; }
    void setTexture2(std::shared_ptr<hhTex2d> newTex) { colorTex2 = newTex; }
    void setNormMap(std::shared_ptr<hhTex2d> newMap) { normalMap = newMap; }
    void setDepthMap(std::shared_ptr<hhTex2d> newMap) { depthMap = newMap; }
    void setSpecMap(std::shared_ptr<hhTex2d> newMap) { specularMap = newMap; }

    // Getters
    float getRoughness() const { return roughness; }
    float getMetallic() const { return metallic; }
    glm::vec3 getEmission() const { return emissiveCol; }
    float getOpacity() const { return opacity; }
    float getRefractiveIndex() const { return refractiveIndex; }

    std::shared_ptr<hhTex2d> getColorTex1() const { return colorTex1; }
    std::shared_ptr<hhTex2d> getColorTex2() const { return colorTex2; }
    std::shared_ptr<hhTex2d> getNormMap() const { return normalMap; }
    std::shared_ptr<hhTex2d> getDepthMap() const { return depthMap; }
    std::shared_ptr<hhTex2d> getSpecMap() const { return specularMap; }

};

#endif