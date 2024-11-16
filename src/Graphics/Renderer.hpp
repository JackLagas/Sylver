#ifndef SYLVER_RENDERER_HPP
#define SYLVER_RENDERER_HPP

#include <Graphics/Window.hpp>

#include <Settings/Settings.hpp>
#include "Renderer/Common/Object.hpp"

namespace Sylver {
    class Renderer {
        public:
            virtual ~Renderer(){};

            virtual void BeginFrame() = 0;
            virtual void Submit(const Object& object) = 0;
            virtual void EndFrame() = 0;

            static Renderer* Create(Window* window, Settings::sRenderer::RendererType backend = Settings::sRenderer::RendererType::OPENGL);
    };
}    // namespace Sylver

#endif
