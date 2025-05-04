#pragma once

#include <Graphics/Renderer.hpp>
#include "Backend/VulkanContext.hpp"
#include "VulkanShader.hpp"
#include <Graphics/Renderer/MVP.hpp>


namespace Sylver {
    class VulkanRenderer : public Renderer {
        private:
            VulkanContext m_Context{};
            VulkanShader* m_SolidShader{nullptr};
            MVP m_MVP;
        public:
            VulkanRenderer(u32 width, u32 height, const Config& cfg);
            ~VulkanRenderer();

            bool BeginFrame() override;
            bool Draw(const VertexArray& vertexArray, const Texture* texture) override;
            bool EndFrame() override;



        private:
            static void SetFramebufferSize(u32 x, u32 y, void* userPtr);
            void Clean();
    };
}    // namespace Sylver
