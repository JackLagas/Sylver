#ifndef SYLVER_GLFW_WINDOW_HPP
#define SYLVER_GLFW_WINDOW_HPP

#include <Graphics/Window.hpp>

#include <vulkan/vulkan.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Settings/Settings.hpp>

namespace Sylver {
    class GLFW_Window : public Window {
        private:
            GLFWwindow* m_WindowHandle;

        public:
            GLFW_Window(std::string title, u32 width, u32 height, Settings::sRenderer::RendererType backend = Settings::sRenderer::RendererType::VULKAN);
            ~GLFW_Window();

            glm::uvec2 GetFramebufferSize() const override;
            u32 GetHeight() const override;
            u32 GetWidth() const override;

            void Update() override;
            b8 ShouldClose() const override;

            void SetFramebufferCallback(std::function<void(u32, u32, void*)> callback) override;
            void SetUserPtr(void* userPtr) override;
            void* GetUserPtr() override;

            void WaitForEvent() override;

            std::vector<const char*> GetRequiredExtensions() const override;
            VkResult CreateWindowSurface(VkInstance instance, const VkAllocationCallbacks* allocationCallback, VkSurfaceKHR* surface) override;
            b8 LoadGlad() override;



    };
}    // namespace Sylver

#endif
