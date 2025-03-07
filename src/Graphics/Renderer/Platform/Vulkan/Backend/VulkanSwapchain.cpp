#include "VulkanContext.hpp"

#include <Core/Logger.hpp>

namespace Sylver {
    void VulkanContext::RecreateSwapchain() {

        auto [width, height] = m_Window->GetFramebufferSize();
        // TODO: Requires testing
        while (width == 0 || height == 0) {
            auto [width, height] = m_Window->GetFramebufferSize();
            m_Window->WaitForEvent();
        }
        vkDeviceWaitIdle(Device);

        CleanupSwapchain();
        CreateSwapchain();
        CreateImageViews();
        CreateFramebuffers();
    }
    bool VulkanContext::CreateSwapchain() {
        SwapchainSupportDetails swapchainSupport = QuerySwapchainSupport(PhysicalDevice);

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapchainSupport.formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapchainSupport.presentModes);
        VkExtent2D extent = ChooseSwapExtent(swapchainSupport.capabilities);

        u32 imageCount = swapchainSupport.capabilities.minImageCount + 1;
        if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount) {
            imageCount = swapchainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = Surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = FindQueueFamilies(PhysicalDevice);
        u32 queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;        // Optional
            createInfo.pQueueFamilyIndices = nullptr;    // Optional
        }
        createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(Device, &createInfo, Allocator, &Swapchain) != VK_SUCCESS) {
            Logger::Critical("Failed to create VkSwapchainKHR");
            return false;
        }
        VULKAN_CREATED("VkSwapchainKHR");

        vkGetSwapchainImagesKHR(Device, Swapchain, &imageCount, nullptr);
        SwapchainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(Device, Swapchain, &imageCount, SwapchainImages.data());

        SwapchainImageFormat = surfaceFormat.format;
        SwapchainExtent = extent;

        return true;
    }
    bool VulkanContext::CreateImageViews() {
        SwapchainImageViews.resize(SwapchainImages.size());
        for (usize i = 0; i < SwapchainImages.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = SwapchainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = SwapchainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(Device, &createInfo, Allocator, &SwapchainImageViews[i]) != VK_SUCCESS) {
                Logger::Critical("Failed to create VkImageView");
                return false;
            }
            VULKAN_CREATED("VkImageView");
        }
        return true;
    }

    bool VulkanContext::CreateFramebuffers() {
        SwapchainFramebuffers.resize(SwapchainImageViews.size());

        for (usize i = 0; i < SwapchainImageViews.size(); i++) {
            VkImageView attachments[] = {
                SwapchainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = RenderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = SwapchainExtent.width;
            framebufferInfo.height = SwapchainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(Device, &framebufferInfo, Allocator, &SwapchainFramebuffers[i]) != VK_SUCCESS) {
                Logger::Critical("Failed to create VkFramebuffer");
                return false;
            }
            VULKAN_CREATED("VkFramebuffer");
        }

        return true;
    }
    void VulkanContext::CleanupSwapchain() {

        for (auto framebuffer : SwapchainFramebuffers) {
            vkDestroyFramebuffer(Device, framebuffer, Allocator);
        }
        for (auto imageView : SwapchainImageViews) {
            vkDestroyImageView(Device, imageView, Allocator);
        }
        vkDestroySwapchainKHR(Device, Swapchain, Allocator);
    }
}    // namespace Sylver
