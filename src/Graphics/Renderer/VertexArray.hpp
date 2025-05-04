#pragma once

#include <Defines.hpp>

#include "Vertex.hpp"

namespace Sylver{
    struct VertexArray{
        std::vector<Vertex> Vertices;
        std::vector<u32> Indices;

        inline static VertexArray Create(glm::vec2 pos, glm::vec2 size, glm::vec4 color = {0.0f, 0.0f, 0.0f, 1.0f}){
            return VertexArray{
                {
                    {{pos.x         , pos.y         }, {0.0f, 0.0f}, color},
                    {{pos.x + size.x, pos.y         }, {1.0f, 0.0f}, color},
                    {{pos.x         , pos.y + size.x}, {0.0f, 1.0f}, color},
                    {{pos.x + size.x, pos.y + size.x}, {1.0f, 1.0f}, color}
                },
                {0, 1, 2, 2, 1, 3}
            };
        }
    };
}
