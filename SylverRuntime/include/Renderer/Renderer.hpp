#pragma once

#include <Defines.hpp>

#include <Config/Config.hpp>
#include "RenderSurface.hpp"
#include "RendererBackend.hpp"


namespace Sylver {
    class Renderer {
        private:
            RendererBackend* m_Backend{};
        public:
            Renderer(const Config& cfg, RenderSurface* surface);
            ~Renderer();
    };
}    // namespace Sylver
