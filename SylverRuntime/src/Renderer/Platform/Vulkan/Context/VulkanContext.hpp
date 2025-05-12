#pragma once

#include <Defines.hpp>
#include <vulkan/vulkan.h>

namespace Sylver{
    struct VulkanContext{
        VkInstance Instance{};
#if defined(SYLVER_DEBUG)
        VkDebugUtilsMessengerEXT DebugMessenger{};
#endif
        VkPhysicalDevice PhysicalDevice{};
        VkDevice Device{};
        VkSurfaceKHR Surface{};
        VkQueue GraphicsQueue{};
        VkQueue PresentQueue{};
        VkSwapchainKHR Swapchain{};
        std::vector<VkImage> SwapchainImages{};
        VkFormat SwapchainImageFormat{};
        VkExtent2D SwapchainExtent{};
        std::vector<VkImageView> SwapchainImageViews{};
        std::vector<VkFramebuffer> SwapchainFramebuffers{};
        VkCommandPool CommandPool{};
        std::vector<VkCommandBuffer> CommandBuffers{};
        VkRenderPass RenderPass{};
        VkImage DepthImage{};
        VkDeviceMemory DepthImageMemory{};
        VkImageView DepthImageView{};
        u32 CurrentFrame{0};
    };
}
