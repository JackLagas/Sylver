#pragma once

#include <Window/Window.hpp>

#include <vulkan/vulkan.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Sylver {
    class GLFW_Window : public Window {
        private:
            u32 m_Width{0};
            u32 m_Height{0};
            GLFWwindow* m_WindowHandle;
            Config::eRenderer m_Backend;

        public:
            GLFW_Window(const std::string& title, const Config& cfg);
            ~GLFW_Window();

            glm::uvec2 GetFramebufferSize() const override;
            u32 GetHeight() const override;
            u32 GetWidth() const override;

            void Update() override;
            bool ShouldClose() const override;

            void WaitForEvent() override;

            RenderSurface* GetSurface() override;


            void SetFramebufferSize(u32 width, u32 height);
    };
}    // namespace Sylver
