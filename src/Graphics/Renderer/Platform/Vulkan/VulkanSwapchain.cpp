#include "VulkanSwapchain.hpp"


namespace Sylver {

    void VulkanSwapchain::Init(SwapchainSupportDetails details, vk::SurfaceKHR surface, Window* window) {
        m_Surface = surface;
        m_Window = window;

        vk::SurfaceFormatKHR chosenFormat = details.Formats[0];
        vk::PresentModeKHR chosenPresentMode = vk::PresentModeKHR::eFifo;
        vk::Extent2D chosenExtent;
        u32 imageCount;
        for (const auto format : details.Formats) {
            if (format.format == vk::Format::eB8G8R8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                chosenFormat = format;
                break;
            }
        }
        for (const auto& presentMode : details.PresentModes) {
            if (presentMode == vk::PresentModeKHR::eMailbox) {
                chosenPresentMode = vk::PresentModeKHR::eMailbox;
                break;
            }
        }

        if (details.Capabilities.currentExtent.width != std::numeric_limits<u32>::max()) {
            chosenExtent = details.Capabilities.currentExtent;
        } else {
            glm::uvec2 framebuffer = window->GetFramebufferSize();
            chosenExtent = vk::Extent2D{
                std::clamp(framebuffer.x, details.Capabilities.minImageExtent.width, details.Capabilities.maxImageExtent.width),
                std::clamp(framebuffer.y, details.Capabilities.minImageExtent.height, details.Capabilities.maxImageExtent.height)
            };
        }
        imageCount = details.Capabilities.minImageCount + 1;
        if (details.Capabilities.maxImageCount > 0 && imageCount > details.Capabilities.maxImageCount) {
            imageCount = details.Capabilities.maxImageCount;
        }
        vk::SwapchainCreateInfoKHR createInfo{};
        createInfo.surface = m_Surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = chosenFormat.format;
        createInfo.imageColorSpace = chosenFormat.colorSpace;
        createInfo.imageExtent = chosenExtent;
    }
}    // namespace Sylver
