#include "Window.hpp"

#include <Graphics/Window/Platform/GLFW/GLFW_Window.hpp>


namespace Sylver {

    Window* Window::Create(const std::string& title, u32 width, u32 height, const Config& cfg) {
#if defined(SYLVER_PLATFORM_WINDOWS)
        return new GLFW_Window(title, width, height, cfg);
#elif defined(SYLVER_PLATFORM_LINUX)
        return new GLFW_Window(title, width, height, cfg);
#elif defined(SYLVER_PLATFORM_MAC)
        return new GLFW_Window(title, width, height, cfg);
#endif
    }
}    // namespace Sylver
