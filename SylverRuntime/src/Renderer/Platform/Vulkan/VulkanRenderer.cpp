#include "VulkanRenderer.hpp"

#include <IO/Logger.hpp>
#include <Config/AppInfo.hpp>
#include <IO/Files.hpp>


namespace Sylver {
    VulkanRenderer::VulkanRenderer(const Config& cfg, RenderSurface* surface) {
    }

    VulkanRenderer::~VulkanRenderer() {
    }

    bool VulkanRenderer::BeginFrame() {
        return true;
    }
    bool VulkanRenderer::Draw(const VertexArray& vertArray){
        return true;
    }

    bool VulkanRenderer::EndFrame() {
        return true;
    }


}    // namespace Sylver
