#ifndef SYLVER_RENDERER_VERTEX_HPP
#define SYLVER_RENDERER_VERTEX_HPP

#include <Defines.hpp>
#include <glm/glm.hpp>

namespace Sylver {
    struct Vertex {
            inline static u32 Stride = 4 * 8;
            glm::vec2 Position;
            glm::vec4 Color;
            glm::vec2 TexCoord;
    };
}    // namespace Sylver


#endif
