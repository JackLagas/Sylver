#include "Window.hpp"

#include <Graphics/Window/Platform/GLFW/GLFW_Window.hpp>

#include "Event.hpp"

namespace Sylver {

    Window* Window::Create(std::string title, u32 width, u32 height) {
#if defined(SYLVER_PLATFORM_WINDOWS)
        return GLFW_Window(title, width, height)
#elif defined(SYLVER_PLATFORM_LINUX)
        return new GLFW_Window(title, width, height);
#elif defined(SYLVER_PLATFORM_MAC)
        return GLFW_Window(title, width, height)
#endif
    }
}    // namespace Sylver
