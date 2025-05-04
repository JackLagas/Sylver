#pragma once
#include <Defines.hpp>
#include <Core/Config.hpp>

#include <vulkan/vulkan.h>
#include <glm/vec2.hpp>


namespace Sylver {
    class Window {
        public:
            static Window* Create(const std::string& title, u32 width, u32 height, const Config& cfg);

            virtual ~Window(){};

            virtual glm::uvec2 GetFramebufferSize() const = 0;
            virtual u32 GetWidth() const = 0;
            virtual u32 GetHeight() const = 0;

            virtual void Update() = 0;
            virtual bool ShouldClose() const = 0;

            virtual void SetFramebufferCallback(std::function<void(u32, u32, void*)> callback) = 0;
            virtual void SetUserPtr(void* userPtr) = 0;
            virtual void* GetUserPtr() = 0;

            virtual void WaitForEvent() = 0;

            virtual std::vector<const char*> GetRequiredExtensions() const = 0;
            virtual VkResult CreateWindowSurface(VkInstance, const VkAllocationCallbacks*, VkSurfaceKHR*) = 0;
            virtual bool LoadGlad() = 0;
    };
}    // namespace Sylver
