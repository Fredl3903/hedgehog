#version 430 core

uniform vec3 camPos;
uniform mat4 modelMat;

uniform sampler2D modelTex1;
uniform sampler2D modelTex2;
uniform sampler2D depthMap;

uniform float focus;      // linear depth where objects are sharp
uniform vec2 texelSize;
const float focusRange = 25.0; // how far from focus before blur starts
const float nearPlane = 0.1;  // for linearizing depth
const float farPlane = 100.0;

in vec3 vertPos;
in vec3 vertColor;
in vec2 vertTex;
out vec4 FragColor;

// --- Linearize the depth value
float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

// --- Compute Gaussian weight
float gaussian(float x, float sigma)
{
    return exp(-0.5 * (x * x) / (sigma * sigma));
}

// --- 1D blur in X and Y
vec4 blurDoF(vec2 uv, float blurAmount)
{
    float sigma = mix(0.5, 8.0, blurAmount); // control spread
    int radius = int(ceil(3.0 * sigma));

    vec4 color = vec4(0.0);
    float sum = 0.0;

    for (int x = -radius; x <= radius; ++x) {
        for (int y = -radius; y <= radius; ++y) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            float weight = gaussian(length(offset), sigma);
            color += texture(modelTex1, uv + offset) * weight;
            sum += weight;
        }
    }

    return color / sum;
}

void main()
{
    float rawDepth = texture(depthMap, vertTex).r;
    float linearDepth = linearizeDepth(rawDepth);

    float blur = clamp(abs(linearDepth - focus) / focusRange, 0.0, 1.0);

    vec4 blurredColor = blurDoF(vertTex, blur);
    FragColor = vec4(vertColor, 1.0) * blurredColor;

    // FragColor = vec4(linearDepth, 0.0, 1.0, 1.0);

}
