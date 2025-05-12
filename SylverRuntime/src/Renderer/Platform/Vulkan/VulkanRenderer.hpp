#pragma once

#include <Renderer/RendererBackend.hpp>


namespace Sylver {
    class VulkanRenderer : public RendererBackend {
        public:
            VulkanRenderer(const Config& cfg, RenderSurface* surface);
            ~VulkanRenderer();

            bool BeginFrame() override;
            bool Draw(const VertexArray& vertArray) override;
            bool EndFrame() override;



    };
}    // namespace Sylver
