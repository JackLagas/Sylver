#include "Renderer.hpp"

#include "Renderer/Platform/Vulkan/VulkanRenderer.hpp"
#include "Renderer/Platform/OpenGL/OpenGLRenderer.hpp"

namespace Sylver {
    Renderer* Renderer::Create(Window* window, Settings::sRenderer::RendererType backend) {
        if (backend == Settings::sRenderer::RendererType::VULKAN) {
            return new VulkanRenderer(window);
        } else if (backend == Settings::sRenderer::RendererType::OPENGL) {
            return new OpenGLRenderer(window);
        }
        return new OpenGLRenderer(window);
    }
}    // namespace Sylver
