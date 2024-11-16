#ifndef VULKAN_SWAPCHAIN_HPP
#define VULKAN_SWAPCHAIN_HPP

#include <Defines.hpp>

#include <vulkan/vulkan.hpp>
#include <Graphics/Window.hpp>

namespace Sylver {
    struct SwapchainSupportDetails {
            vk::SurfaceCapabilitiesKHR Capabilities;
            std::vector<vk::SurfaceFormatKHR> Formats;
            std::vector<vk::PresentModeKHR> PresentModes;
    };

    class VulkanSwapchain {
        private:
            vk::SwapchainKHR m_Swapchain{};
            vk::SurfaceFormatKHR m_SurfaceFormat{};
            vk::PresentModeKHR m_PresentMode{};
            vk::Extent2D m_Extent{};
            vk::SurfaceKHR m_Surface{};
            Window* m_Window;

        public:
            void Init(SwapchainSupportDetails details, vk::SurfaceKHR surface, Window* window);
    };
}    // namespace Sylver


#endif
