#ifndef SYLVER_GLFW_WINDOW_HPP
#define SYLVER_GLFW_WINDOW_HPP

#include <Graphics/Window.hpp>

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <Settings/Settings.hpp>

namespace Sylver {
    class GLFW_Window : public Window {
        private:
            GLFWwindow* m_WindowHandle;

        public:
            GLFW_Window(std::string title, u32 width, u32 height, Settings::sRenderer::RendererType backend = Settings::sRenderer::RendererType::OPENGL);
            ~GLFW_Window();

            std::vector<const char*> GetRequiredExtensions();
            vk::Result CreateWindowSurface(vk::Instance instance, const vk::AllocationCallbacks* allocationCallback, vk::SurfaceKHR* surface);


            glm::uvec2 GetFramebufferSize();
            u32 GetHeight();
            u32 GetWidth();
            void Update();

            b8 ShouldClose() {
                return glfwWindowShouldClose(m_WindowHandle);
            }
            b8 LoadGlad();
    };
}    // namespace Sylver

#endif
