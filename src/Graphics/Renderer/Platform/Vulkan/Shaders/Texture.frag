#version 450
layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec2 TexCoords;
layout (location = 1) in vec4 ourColor;

layout (binding = 0) uniform sampler2D ourTexture;

void main(){
    FragColor = texture(ourTexture, TexCoords);
}

