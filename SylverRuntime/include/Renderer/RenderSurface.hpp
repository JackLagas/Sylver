#pragma once
#include <Defines.hpp>
#include <Config/Config.hpp>
#include <glm/vec2.hpp>


namespace Sylver {
    class RenderSurface {
        public:
            virtual ~RenderSurface(){};

            virtual const glm::uvec2 GetFramebufferSize() const = 0;
            virtual const u32 GetWidth() const = 0;
            virtual const u32 GetHeight() const = 0;

            virtual void WaitForEvent() = 0;
    };
}    // namespace Sylver
