#include "GLFW_Window.hpp"

#include <IO/Logger.hpp>

#include <Event/Events.hpp>
#include <Event/Keys.hpp>
#include "GLFW_Keys.hpp"

#include <Renderer/RenderSurface_OpenGL.hpp>
#include <Renderer/RenderSurface_Vulkan.hpp>

#include <glad/glad.h>


namespace Sylver {

    class GLFW_RenderSurface_OpenGL : public RenderSurface_OpenGL {
        private:
            u32& m_Width;
            u32& m_Height;
            GLFWwindow* m_WindowHandle{nullptr};

        public:
            GLFW_RenderSurface_OpenGL(GLFWwindow* windowHandle, u32& width, u32& height): 
                m_Width(width), 
                m_Height(height), 
                m_WindowHandle(windowHandle){}
            ~GLFW_RenderSurface_OpenGL(){
                
            }

            const glm::uvec2 GetFramebufferSize() const override{
                return {m_Width, m_Height};
            }
            const u32 GetWidth() const override {
                return m_Width;
            }
            const u32 GetHeight() const override {
                return m_Height;
            }
            void WaitForEvent() override {
                glfwWaitEvents();
            }
            void LoadOpenGL() override {
                gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            }
    };
    class GLFW_RenderSurface_Vulkan : public RenderSurface_Vulkan {
        private:
            u32& m_Width;
            u32& m_Height;
            GLFWwindow* m_WindowHandle{nullptr};

        public:
            GLFW_RenderSurface_Vulkan(GLFWwindow* windowHandle, u32& width, u32& height): 
                m_Width(width), 
                m_Height(height), 
                m_WindowHandle(windowHandle){}
            ~GLFW_RenderSurface_Vulkan(){
                
            }

            const glm::uvec2 GetFramebufferSize() const override{
                return {m_Width, m_Height};
            }
            const u32 GetWidth() const override {
                return m_Width;
            }
            const u32 GetHeight() const override {
                return m_Height;
            }
            void WaitForEvent() override {
                glfwWaitEvents();
            }
            std::vector<const char*> GetRequiredExtensions() const override{
                u32 count;
                const char** extensions = glfwGetRequiredInstanceExtensions(&count);
                std::vector<const char*> extensionVector(extensions, extensions + count);
                return extensionVector;
            }
            VkResult CreateWindowSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) override{
                return glfwCreateWindowSurface(
                    instance,
                    m_WindowHandle,
                    allocator,
                    surface);
            }
    };

    //
    // Event Callbacks
    //
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        EventListener<KeyboardEvent>::AddEvent(KeyboardEvent{
            .window_id = (void*)window,
            .key = GLFWKeysToSylverKeys(key),
            .action = GLFWActionToSylverAction(action),
        });
    }
    static void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
        EventListener<MouseMoveEvent>::AddEvent(MouseMoveEvent{
            .window_id = (void*)window,
            .x = xpos,
            .y = ypos });
    }
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        EventListener<MouseClickEvent>::AddEvent(MouseClickEvent{
            .window_id = (void*)window,
            .button = GLFWMouseButtonToSylverMouseButton(button),
            .action = GLFWActionToSylverAction(action) });
    }
    static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        EventListener<MouseScrollEvent>::AddEvent(MouseScrollEvent{
            .window_id = (void*)window,
            .x_offset = xoffset,
            .y_offset = yoffset,
        });
    }
    static void WindowCloseCallback(GLFWwindow* window) {
        EventListener<WindowCloseEvent>::AddEvent(WindowCloseEvent{
            .window_id = (void*)window,
        });
    }
    static void WindowSizeCallback(GLFWwindow* window, int width, int height) {
        EventListener<WindowResizeEvent>::AddEvent(WindowResizeEvent{
            .window_id = (void*)window,
            .x = static_cast<u32>(width),
            .y = static_cast<u32>(height) });
    }
    static void WindowMoveCallback(GLFWwindow* window, int xpos, int ypos) {
        EventListener<WindowMoveEvent>::AddEvent(WindowMoveEvent{
            .window_id = (void*)window,
            .x = static_cast<u32>(xpos),
            .y = static_cast<u32>(ypos) });
    }
    static void WindowMinimizeCallback(GLFWwindow* window, int iconified) {
        EventListener<WindowMinimizeEvent>::AddEvent(WindowMinimizeEvent{
            .window_id = (void*)window,
            .minimized = static_cast<u32>(iconified) });
    }
    static void WindowMaximizeCallback(GLFWwindow* window, int maximized) {
        EventListener<WindowMaximizeEvent>::AddEvent(WindowMaximizeEvent{
            .window_id = (void*)window,
            .maximized = static_cast<u32>(maximized) });
    }
    static void WindowFocusCallback(GLFWwindow* window, int focused) {
        EventListener<WindowFocusEvent>::AddEvent(WindowFocusEvent{
            .window_id = (void*)window,
            .gained_focus = static_cast<u32>(focused) });
    }

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
        GLFW_Window* appWindow = (GLFW_Window*)glfwGetWindowUserPointer(window);
        appWindow->SetFramebufferSize(width, height);
    }


    GLFW_Window::GLFW_Window(const std::string& title, const Config& cfg): m_Backend(cfg.Renderer.Backend), m_Width(cfg.Renderer.Width), m_Height(cfg.Renderer.Height) {
        glfwInit();

        if (cfg.Renderer.Backend == Config::eRenderer::VULKAN) {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        } else if (cfg.Renderer.Backend == Config::eRenderer::OPENGL) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(SYLVER_PLATFORM_MAC)
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        }
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

        m_WindowHandle = glfwCreateWindow(static_cast<i32>(m_Width), static_cast<i32>(m_Height), title.c_str(), nullptr, nullptr);

        if (m_WindowHandle == nullptr) {
            Logger::Critical("Failed to create window");
        }
        if (cfg.Renderer.Backend == Config::eRenderer::OPENGL) {
            glfwMakeContextCurrent(m_WindowHandle);
        }
        glfwSetWindowUserPointer(m_WindowHandle, this);
        glfwSetKeyCallback(m_WindowHandle, KeyCallback);
        glfwSetMouseButtonCallback(m_WindowHandle, MouseButtonCallback);
        glfwSetCursorPosCallback(m_WindowHandle, MouseMoveCallback);
        glfwSetScrollCallback(m_WindowHandle, MouseScrollCallback);
        glfwSetWindowCloseCallback(m_WindowHandle, WindowCloseCallback);
        glfwSetWindowSizeCallback(m_WindowHandle, WindowSizeCallback);
        glfwSetWindowPosCallback(m_WindowHandle, WindowMoveCallback);
        glfwSetWindowIconifyCallback(m_WindowHandle, WindowMinimizeCallback);
        glfwSetWindowMaximizeCallback(m_WindowHandle, WindowMaximizeCallback);
        glfwSetWindowFocusCallback(m_WindowHandle, WindowFocusCallback);
        glfwSetFramebufferSizeCallback(m_WindowHandle, FramebufferSizeCallback);
    }
    GLFW_Window::~GLFW_Window() {
        glfwDestroyWindow(m_WindowHandle);

        glfwTerminate();
    }

    //
    // Renderer Helpers
    //


    void GLFW_Window::WaitForEvent() {
        glfwWaitEvents();
    }
    glm::uvec2 GLFW_Window::GetFramebufferSize() const {
        i32 width, height;
        glfwGetFramebufferSize(m_WindowHandle, &width, &height);
        return glm::uvec2(
            static_cast<u32>(width),
            static_cast<u32>(height));
    }
    u32 GLFW_Window::GetHeight() const {
        return GetFramebufferSize().y;
    }
    u32 GLFW_Window::GetWidth() const {

        return GetFramebufferSize().x;
    }

    bool GLFW_Window::ShouldClose() const {
        return glfwWindowShouldClose(m_WindowHandle);
    }

    void GLFW_Window::Update() {
        glfwSwapBuffers(m_WindowHandle);
        glfwPollEvents();
    }
    RenderSurface* GLFW_Window::GetSurface(){
        if(m_Backend == Config::eRenderer::OPENGL){
            return new GLFW_RenderSurface_OpenGL(m_WindowHandle, m_Width, m_Height);
        }
        if(m_Backend == Config::eRenderer::VULKAN){
            return new GLFW_RenderSurface_Vulkan(m_WindowHandle, m_Width, m_Height);
        }
        return nullptr;
    }

    void GLFW_Window::SetFramebufferSize(u32 width, u32 height){
        m_Width = width;
        m_Height = height;
    }
}    // namespace Sylver
