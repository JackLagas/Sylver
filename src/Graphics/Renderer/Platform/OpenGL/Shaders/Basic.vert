#version 450
layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTexCoords;
layout (location = 2) in vec4 inColor;

layout (location = 0) out vec2 TexCoords;
layout (location = 1) out vec4 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
    gl_Position = proj * view * model * vec4(inPos, 0.0, 1.0);
    gl_Position.z = 0.0;
    ourColor = inColor;
    TexCoords = inTexCoords;
}
