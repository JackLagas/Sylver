#ifndef SYLVER_WINDOW_HPP
#define SYLVER_WINDOW_HPP

#include <Defines.hpp>

#include <vulkan/vulkan.hpp>
#include <glm/vec2.hpp>


namespace Sylver {
    class Window {
        public:
            static Window* Create(std::string title, u32 width, u32 height);

            virtual ~Window(){};

            virtual glm::uvec2 GetFramebufferSize() = 0;
            virtual u32 GetWidth() = 0;
            virtual u32 GetHeight() = 0;

            virtual void Update() = 0;
            virtual b8 ShouldClose() = 0;

            virtual std::vector<const char*> GetRequiredExtensions() = 0;
            virtual vk::Result CreateWindowSurface(vk::Instance, const vk::AllocationCallbacks*, vk::SurfaceKHR*) = 0;
            virtual b8 LoadGlad() = 0;
    };
}    // namespace Sylver

#endif
