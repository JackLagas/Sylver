#pragma once

#include <Graphics/Window.hpp>

#include <vulkan/vulkan.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Sylver {
    class GLFW_Window : public Window {
        private:
            GLFWwindow* m_WindowHandle;

        public:
            GLFW_Window(const std::string& title, u32 width, u32 height, const Config& cfg);
            ~GLFW_Window();

            glm::uvec2 GetFramebufferSize() const override;
            u32 GetHeight() const override;
            u32 GetWidth() const override;

            void Update() override;
            bool ShouldClose() const override;

            void SetFramebufferCallback(std::function<void(u32, u32, void*)> callback) override;
            void SetUserPtr(void* userPtr) override;
            void* GetUserPtr() override;

            void WaitForEvent() override;

            std::vector<const char*> GetRequiredExtensions() const override;
            VkResult CreateWindowSurface(VkInstance instance, const VkAllocationCallbacks* allocationCallback, VkSurfaceKHR* surface) override;
            bool LoadGlad() override;
    };
}    // namespace Sylver
