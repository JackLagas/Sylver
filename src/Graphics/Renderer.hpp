#pragma once

#include <Defines.hpp>

#include <Graphics/Window.hpp>

#include <Core/Config.hpp>
#include "Renderer/VertexArray.hpp"
#include "Renderer/Texture.hpp"


namespace Sylver {
    class Renderer {
        protected:
            Window* m_Window{ nullptr };

        public:
            virtual ~Renderer() {
                delete m_Window;
            };

            virtual bool BeginFrame() = 0;
            virtual bool Draw(const VertexArray& vertexArray, const Texture* texture) = 0;
            virtual bool EndFrame() = 0;

            bool ShouldClose() const {
                return m_Window->ShouldClose();
            }


            static Renderer* Create(const Config& cfg);
    };
}    // namespace Sylver
