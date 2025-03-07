#pragma once

#include <Defines.hpp>

#include <Graphics/Window.hpp>

#include <Core/Config.hpp>


namespace Sylver {
    class Renderer {
        protected:
            Window* m_Window{ nullptr };

        public:
            virtual ~Renderer() {
                delete m_Window;
            };

            virtual bool BeginFrame() = 0;
            virtual bool DrawSprite(glm::vec2 pos, glm::vec2 size, u8* texture) = 0;
            virtual bool DrawSprite(f32 x, f32 y, f32 w, f32 h, u8* texture) = 0;
            virtual bool DrawRect(glm::vec2 pos, glm::vec2 size, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }) = 0;
            virtual bool DrawRect(f32 x, f32 y, f32 w, f32 h, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }) = 0;
            virtual bool EndFrame() = 0;

            bool ShouldClose() const {
                return m_Window->ShouldClose();
            }


            static Renderer* Create(const Config& cfg);
    };
}    // namespace Sylver
