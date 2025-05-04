#pragma once

#include <Defines.hpp>

namespace Sylver{
    struct Vertex {
        glm::vec2 Position;
        glm::vec2 TexCoords;
        glm::vec4 Color;
        inline static const sizet Stride(){
            return sizeof(Vertex);
        }
    };
}
