#ifndef SYLVER_VULKAN_RENDERER_HPP
#define SYLVER_VULKAN_RENDERER_HPP

#include <Graphics/Renderer.hpp>
#include "Backend/VulkanContext.hpp"

namespace Sylver {
    class VulkanRenderer : public Renderer {
        private:
            VulkanContext m_Context{};

        public:
            VulkanRenderer(u32 width, u32 height);
            ~VulkanRenderer();

            b8 BeginFrame() override;
            b8 DrawSprite(glm::vec2 pos, glm::vec2 size, u8* texture) override;
            b8 DrawSprite(f32 x, f32 y, f32 w, f32 h, u8* texture) override;
            b8 DrawRect(glm::vec2 pos, glm::vec2 size, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }) override;
            b8 DrawRect(f32 x, f32 y, f32 w, f32 h, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }) override;
            b8 EndFrame() override;


        private:
            static void SetFramebufferSize(u32 x, u32 y, void* userPtr);
            void Clean();
    };
}    // namespace Sylver

#endif
