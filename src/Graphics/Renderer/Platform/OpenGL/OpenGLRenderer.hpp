#pragma once

#include <Graphics/Renderer.hpp>

namespace Sylver {
    class OpenGLRenderer : public Renderer {
        private:
        public:
            OpenGLRenderer(u32 width, u32 height, const Config& cfg);
            ~OpenGLRenderer();

            bool BeginFrame() override;
            bool DrawSprite(glm::vec2 pos, glm::vec2 size, u8* texture) override;
            bool DrawSprite(f32 x, f32 y, f32 w, f32 h, u8* texture) override;
            bool DrawRect(glm::vec2 pos, glm::vec2 size, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }) override;
            bool DrawRect(f32 x, f32 y, f32 w, f32 h, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }) override;
            bool EndFrame() override;
    };
}    // namespace Sylver
