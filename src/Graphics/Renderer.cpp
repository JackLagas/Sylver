#include "Renderer.hpp"

#include "Renderer/Platform/Vulkan/VulkanRenderer.hpp"

namespace Sylver {
    Renderer* Renderer::Create(Settings::sRenderer rendererSettings) {
        if (rendererSettings.Backend == Settings::sRenderer::RendererType::VULKAN) {
            return new VulkanRenderer(rendererSettings.RendererWidth, rendererSettings.RendererHeight);
        }
        return nullptr;
    }
}    // namespace Sylver
