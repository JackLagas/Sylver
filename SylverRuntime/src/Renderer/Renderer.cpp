#include <Renderer/Renderer.hpp>


namespace Sylver{
    Renderer::Renderer(const Config& cfg, RenderSurface* surface){
        m_Backend = RendererBackend::Create(cfg, surface);
    }
    Renderer::~Renderer(){
        delete m_Backend;
    }
}
