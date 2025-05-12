#include "Window.hpp"

#include "Platform/GLFW/GLFW_Window.hpp"


namespace Sylver {

    Window* Window::Create(const std::string& title, const Config& cfg) {
#if defined(SYLVER_PLATFORM_WINDOWS)
        return new GLFW_Window(title, cfg);
#elif defined(SYLVER_PLATFORM_LINUX)
        return new GLFW_Window(title, cfg);
#elif defined(SYLVER_PLATFORM_MAC)
        return new GLFW_Window(title, cfg);
#endif
    }
}    // namespace Sylver
