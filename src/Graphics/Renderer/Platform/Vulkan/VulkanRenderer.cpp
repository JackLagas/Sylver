#include "VulkanRenderer.hpp"

#include <Core/Logger.hpp>
#include <Core/AppInfo.hpp>
#include <vulkan/vulkan_handles.hpp>

namespace Sylver {
    VulkanRenderer::VulkanRenderer(Window* window) {
        vk::ApplicationInfo appInfo(
            AppInfo::Name().c_str(),
            1,
            "SylverEngine",
            VK_MAKE_VERSION(1, 0, 0),
            VK_API_VERSION_1_3);
        auto extensions = window->GetRequiredExtensions();
        for (auto extension : extensions) {
            Logger::Info(extension);
        }
        m_Context.Init(window);


        Logger::Info("Selected Device: {}", m_Context.PhysicalDevice.getProperties().deviceName.data());
    }

    VulkanRenderer::~VulkanRenderer() {
        Clean();
    }
    void VulkanRenderer::BeginFrame() {
    }
    void VulkanRenderer::Submit(const Object& object) {
    }
    void VulkanRenderer::EndFrame() {
    }


    void VulkanRenderer::Clean() {
    }
}    // namespace Sylver
