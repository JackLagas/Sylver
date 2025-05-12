#pragma once
#include "RenderSurface.hpp"
#include <Defines.hpp>
#include <Config/Config.hpp>
#include <glm/vec2.hpp>


namespace Sylver {
    class RenderSurface_OpenGL : public RenderSurface {
        public:
            virtual ~RenderSurface_OpenGL() override {};

            virtual const glm::uvec2 GetFramebufferSize() const override = 0;
            virtual const u32 GetWidth() const override = 0;
            virtual const u32 GetHeight() const override = 0;

            virtual void WaitForEvent() override = 0;

            virtual void LoadOpenGL() = 0;
    };
}    // namespace Sylver
