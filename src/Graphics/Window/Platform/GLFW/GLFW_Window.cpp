#include "GLFW_Window.hpp"

#include <Core/Logger.hpp>

#include <Graphics/Event.hpp>
#include <Graphics/Event/Keys.hpp>
#include "GLFW_Keys.hpp"


namespace Sylver {

    //
    // Event Callbacks
    //
    static std::function<void(u32, u32, void*)> FramebufferCallback = [](u32, u32, void*) {};
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

    void GLFW_Window::SetFramebufferCallback(std::function<void(u32, u32, void*)> callback) {
        FramebufferCallback = callback;
        glfwSetFramebufferSizeCallback(m_WindowHandle, [](GLFWwindow* window, int x, int y) {
            FramebufferCallback(
                static_cast<u32>(x),
                static_cast<u32>(y),
                glfwGetWindowUserPointer(window)
                );
        });
    }


    GLFW_Window::GLFW_Window(std::string title, u32 width, u32 height, Settings::sRenderer::RendererType backend) {
        glfwInit();

        if (backend == Settings::sRenderer::RendererType::VULKAN) {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        } else if (backend == Settings::sRenderer::RendererType::OPENGL) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

        m_WindowHandle = glfwCreateWindow(static_cast<s32>(width), static_cast<s32>(height), title.c_str(), nullptr, nullptr);

        if (m_WindowHandle == nullptr) {
            Logger::Critical("Failed to create window");
        }
        if (backend == Settings::sRenderer::RendererType::OPENGL) {
            glfwMakeContextCurrent(m_WindowHandle);
        }
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
    }
    GLFW_Window::~GLFW_Window() {
        glfwDestroyWindow(m_WindowHandle);

        glfwTerminate();
    }

    //
    // Renderer Helpers
    //


    void GLFW_Window::SetUserPtr(void* userPtr) {
        glfwSetWindowUserPointer(m_WindowHandle, userPtr);
    }
    void* GLFW_Window::GetUserPtr() {
        return glfwGetWindowUserPointer(m_WindowHandle);
    }
    void GLFW_Window::WaitForEvent(){
        glfwWaitEvents();
    }
    std::vector<const char*> GLFW_Window::GetRequiredExtensions() const {
        u32 count;
        const char** extensions = glfwGetRequiredInstanceExtensions(&count);
        std::vector<const char*> extensionVector(extensions, extensions + count);
        return extensionVector;
    }
    VkResult GLFW_Window::CreateWindowSurface(VkInstance instance, const VkAllocationCallbacks* allocationCallback, VkSurfaceKHR* surface) {
        return glfwCreateWindowSurface(
            instance,
            m_WindowHandle,
            allocationCallback,
            surface);
    }

    b8 GLFW_Window::LoadGlad() {
        return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }
    glm::uvec2 GLFW_Window::GetFramebufferSize() const {
        s32 width, height;
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

    b8 GLFW_Window::ShouldClose() const {
        return glfwWindowShouldClose(m_WindowHandle);
    }

    void GLFW_Window::Update() {
        glfwSwapBuffers(m_WindowHandle);
        glfwPollEvents();
    }
}    // namespace Sylver
