#pragma once

#include <Renderer/RendererBackend.hpp>

namespace Sylver {
    class OpenGLRenderer : public RendererBackend {
        private:
        public:
            OpenGLRenderer(const Config& cfg, RenderSurface* surface);
            ~OpenGLRenderer();

            bool BeginFrame() override;
            bool Draw(const VertexArray& vertArray) override;
            bool EndFrame() override;
    };
}    // namespace Sylver
