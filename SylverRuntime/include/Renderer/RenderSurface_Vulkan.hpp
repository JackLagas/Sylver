#pragma once
#include "RenderSurface.hpp"
#include <Defines.hpp>
#include <Config/Config.hpp>
#include <glm/vec2.hpp>
#include <vulkan/vulkan.h>


namespace Sylver {
    class RenderSurface_Vulkan : public RenderSurface {
        public:
            virtual ~RenderSurface_Vulkan() override {};

            virtual const glm::uvec2 GetFramebufferSize() const override = 0;
            virtual const u32 GetWidth() const override = 0;
            virtual const u32 GetHeight() const override = 0;

            virtual void WaitForEvent() override = 0;

            virtual std::vector<const char*> GetRequiredExtensions() const = 0;
            virtual VkResult CreateWindowSurface(VkInstance instace, const VkAllocationCallbacks* callbacks, VkSurfaceKHR* surface) = 0;
    };
}    // namespace Sylver
