#ifndef SYLVER_RENDERER_HPP
#define SYLVER_RENDERER_HPP

#include <Defines.hpp>

#include <Graphics/Window.hpp>

#include <Settings/Settings.hpp>


namespace Sylver {
    class Renderer {
        protected:
            Window* m_Window{ nullptr };

        public:
            virtual ~Renderer() {
                delete m_Window;
            };

            virtual b8 BeginFrame() = 0;
            virtual b8 DrawSprite(glm::vec2 pos, glm::vec2 size, u8* texture) = 0;
            virtual b8 DrawSprite(f32 x, f32 y, f32 w, f32 h, u8* texture) = 0;
            virtual b8 DrawRect(glm::vec2 pos, glm::vec2 size, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }) = 0;
            virtual b8 DrawRect(f32 x, f32 y, f32 w, f32 h, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }) = 0;
            virtual b8 EndFrame() = 0;

            b8 ShouldClose() const {
                return m_Window->ShouldClose();
            }


            static Renderer* Create(Settings::sRenderer rendererSettings);
    };
}    // namespace Sylver

#endif
