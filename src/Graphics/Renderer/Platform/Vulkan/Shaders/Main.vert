#version 450

layout(binding = 0) uniform MVP {
    mat4 model;
    mat4 view;
    mat4 proj;
} mvp;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoords;
layout(location = 2) in vec4 inColor;

layout(location = 0) out vec2 texCoords;
layout(location = 1) out vec4 fragColor;

void main() {
    gl_Position = mvp.proj * mvp.view * mvp.model * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
    texCoords = inTexCoords;
}
