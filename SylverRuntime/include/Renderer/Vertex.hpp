#pragma once

#include <Defines.hpp>

namespace Sylver{
    struct Vertex{
        glm::vec3 Position{};
        glm::vec4 Color{};
        glm::vec2 TexCoords{};
        u32 TexID{};
    };
}
