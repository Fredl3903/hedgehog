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

uniform float k;

vec3 extractBright(vec3 color, out float alphaOut) {
    float brightness = max(max(color.r, color.g), color.b);
    
    float knee = k * 0.2;
    float soft = smoothstep(k - knee, k + knee, brightness);

    alphaOut = soft; // Pass alpha out to use in FragColor
    return color * soft;
}

void main() {
    vec3 texColor = texture(modelTex1, vertTex).rgb;
    float alpha;
    vec3 brightColor = extractBright(texColor, alpha);
    
    FragColor = vec4(brightColor, alpha);
}