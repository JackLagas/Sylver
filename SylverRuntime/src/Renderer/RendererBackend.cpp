#include <Renderer/RendererBackend.hpp>

#include "Platform/Vulkan/VulkanRenderer.hpp"
#include "Platform/OpenGL/OpenGLRenderer.hpp"
#include <IO/Logger.hpp>

namespace Sylver {
    RendererBackend* RendererBackend::Create(const Config& cfg, RenderSurface* surface) {
        switch(cfg.Renderer.Backend){
            case Config::eRenderer::VULKAN:
                return new VulkanRenderer(cfg, surface);
            case Config::eRenderer::OPENGL:
                return new OpenGLRenderer(cfg, surface);
            case Config::eRenderer::METAL:
            case Config::eRenderer::SOFTWARE:
            case Config::eRenderer::DIRECT3D:
            case Config::eRenderer::NONE:
                Logger::Critical("Renderer type unsupported");
                return nullptr;
        }
    }
}    // namespace Sylver
