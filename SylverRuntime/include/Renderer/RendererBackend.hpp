#pragma once

#include <Defines.hpp>

#include <Config/Config.hpp>
#include "VertexArray.hpp"
#include "RenderSurface.hpp"

namespace Sylver {
    class RendererBackend {
        public:
            virtual bool BeginFrame() = 0;
            virtual bool Draw(const VertexArray& vertArray) = 0;
            virtual bool EndFrame() = 0;


            static RendererBackend* Create(const Config& cfg, RenderSurface* surface);
    };
}    // namespace Sylver
