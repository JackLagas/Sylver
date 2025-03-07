#include "Renderer.hpp"

#include "Renderer/Platform/Vulkan/VulkanRenderer.hpp"
#include "Renderer/Platform/OpenGL/OpenGLRenderer.hpp"

namespace Sylver {
    Renderer* Renderer::Create(const Config& cfg) {
        if (cfg.RendererBackend == Config::eRenderer::VULKAN) {
            return new VulkanRenderer(cfg.RendererWidth, cfg.RendererHeight, cfg);
        }
        if (cfg.RendererBackend == Config::eRenderer::OPENGL) {
            return new OpenGLRenderer(cfg.RendererWidth, cfg.RendererHeight, cfg);
        }
        return nullptr;
    }
}    // namespace Sylver
