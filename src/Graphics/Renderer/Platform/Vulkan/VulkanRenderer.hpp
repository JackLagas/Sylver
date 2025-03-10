#pragma once

#include <Graphics/Renderer.hpp>
#include "Backend/VulkanContext.hpp"

namespace Sylver {
    class VulkanRenderer : public Renderer {
        private:
            VulkanContext m_Context{};

        public:
            VulkanRenderer(u32 width, u32 height, const Config& cfg);
            ~VulkanRenderer();

            bool BeginFrame() override;
            bool DrawSprite(glm::vec2 pos, glm::vec2 size, u8* texture) override;
            bool DrawSprite(f32 x, f32 y, f32 w, f32 h, u8* texture) override;
            bool DrawRect(glm::vec2 pos, glm::vec2 size, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }) override;
            bool DrawRect(f32 x, f32 y, f32 w, f32 h, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }) override;
            bool EndFrame() override;


        private:
            static void SetFramebufferSize(u32 x, u32 y, void* userPtr);
            void Clean();
    };
}    // namespace Sylver
