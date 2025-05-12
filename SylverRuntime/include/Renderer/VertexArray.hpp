#pragma once

#include <Defines.hpp>
#include "Vertex.hpp"

namespace Sylver{
    struct VertexArray{
        std::vector<Vertex> Vertices{};
        std::vector<u32> Indices{};
    };
}
