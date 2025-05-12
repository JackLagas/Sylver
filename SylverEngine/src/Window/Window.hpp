#pragma once
#include <Defines.hpp>
#include <Config/Config.hpp>

#include <vulkan/vulkan.h>
#include <glm/vec2.hpp>

#include <Renderer/RenderSurface.hpp>


namespace Sylver {
    class Window {
        public:
            static Window* Create(const std::string& title, const Config& cfg);

            virtual ~Window(){};

            virtual glm::uvec2 GetFramebufferSize() const = 0;
            virtual u32 GetWidth() const = 0;
            virtual u32 GetHeight() const = 0;

            virtual void Update() = 0;
            virtual bool ShouldClose() const = 0;


            virtual void WaitForEvent() = 0;

            virtual RenderSurface* GetSurface() = 0;
    };
}    // namespace Sylver
