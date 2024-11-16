#include "GLFW_Window.hpp"

#include <Core/Logger.hpp>

#include <glad/glad.h>

namespace Sylver {
    GLFW_Window::GLFW_Window(std::string title, u32 width, u32 height, Settings::sRenderer::RendererType backend) {
        glfwInit();

        if (backend == Settings::sRenderer::RendererType::VULKAN) {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        } else if (backend == Settings::sRenderer::RendererType::OPENGL) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }
        // TODO: Add resize support
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

        m_WindowHandle = glfwCreateWindow(static_cast<s32>(width), static_cast<s32>(height), title.c_str(), nullptr, nullptr);

        if (m_WindowHandle == nullptr) {
            Logger::Critical("Failed to create window");
        }
        if (backend == Settings::sRenderer::RendererType::OPENGL) {
            glfwMakeContextCurrent(m_WindowHandle);
        }
    }
    GLFW_Window::~GLFW_Window() {
        glfwDestroyWindow(m_WindowHandle);

        glfwTerminate();
    }

    //
    // Renderer Helpers
    //


    std::vector<const char*> GLFW_Window::GetRequiredExtensions() {
        u32 count;
        const char** extensions = glfwGetRequiredInstanceExtensions(&count);
        std::vector<const char*> extensionVector(extensions, extensions + count);
        return extensionVector;
    }
    vk::Result GLFW_Window::CreateWindowSurface(vk::Instance instance, const vk::AllocationCallbacks* allocationCallback, vk::SurfaceKHR* surface) {
        return static_cast<vk::Result>(
            glfwCreateWindowSurface(
                (VkInstance)instance,
                m_WindowHandle,
                (const VkAllocationCallbacks*)allocationCallback,
                (VkSurfaceKHR*)surface));
    }

    b8 GLFW_Window::LoadGlad(){
        return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }
    glm::uvec2 GLFW_Window::GetFramebufferSize() {
        s32 width, height;
        glfwGetFramebufferSize(m_WindowHandle, &width, &height);
        return glm::uvec2(
            static_cast<u32>(width),
            static_cast<u32>(height));
    }
    u32 GLFW_Window::GetHeight() {
        return GetFramebufferSize().y;
    }
    u32 GLFW_Window::GetWidth() {

        return GetFramebufferSize().x;
    }

    void GLFW_Window::Update() {
        glfwSwapBuffers(m_WindowHandle);
        glfwPollEvents();
    }
}    // namespace Sylver
