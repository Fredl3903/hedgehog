#version 430 core

uniform vec3 camPos;
uniform mat4 modelMat;

uniform sampler2D modelTex1;
uniform sampler2D modelTex2;
uniform sampler2D depthMap;

in vec3 vertPos;
in vec3 vertColor;
in vec2 vertTex;
out vec4 FragColor;

uniform float blur;
uniform vec2 texelSize;


vec4 verticalBlur(vec2 uv) {
    float sigma = mix(0.5, 10.0, blur);
    int radius = int(ceil(3.0 * sigma));

    float sum = 0.0;
    vec4 color = vec4(0.0);
    
    for (int i = -radius; i <= radius; ++i) {
        float x = float(i);
        float weight = exp(-0.5 * (x * x) / (sigma * sigma));
        vec2 offset = vec2(0.0, x) * texelSize;
        color += texture(modelTex1, uv + offset) * weight;
        sum += weight;
    }

    return color / sum;
}


void main() {

  vec4 texColor = verticalBlur(vertTex);
  FragColor = vec4(vertColor, 1.0) * texColor;

}