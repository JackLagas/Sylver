#include "VulkanContext.hpp"

#include <Core/AppInfo.hpp>

#include "VulkanShared.hpp"
#include "VulkanDebug.hpp"

namespace Sylver {

    bool VulkanContext::Init(Window* window) {
#if !defined(NDEBUG) && defined(SYLVER_USE_CUSTOM_VULKAN_ALLOCATOR)
        Allocator = new VkAllocationCallbacks();
        Allocator->pfnAllocation = VulkanAllocation;
        Allocator->pfnReallocation = VulkanReallocation;
        Allocator->pfnFree = VulkanFree;
#endif
        m_Window = window;
        if (!CreateInstance()) {
            return false;
        }
        if (enableValidationLayers) {
            if (!SetupDebugMessenger()) {
                return false;
            }
        }

        if (m_Window->CreateWindowSurface(Instance, Allocator, &Surface) != VK_SUCCESS) {
            Logger::Critical("Failed to create VkSurfaceKHR");
            return false;
        }
        Buffers.resize(MAX_FRAMES_IN_FLIGHT);

        if (!PickPhysicalDevice()) {
            return false;
        }
        if (!CreateLogicalDevice()) {
            return false;
        }
        if (!CreateSwapchain()) {
            return false;
        }
        if (!CreateImageViews()) {
            return false;
        }
        if (!CreateRenderPass()) {
            return false;
        }
        /*
        if (!CreateGraphicsPipeline()) {
            return false;
        }
        */
        if (!CreateFramebuffers()) {
            return false;
        }
        if (!CreateCommandPool()) {
            return false;
        }
        if (!CreateCommandBuffer()) {
            return false;
        }
        if (!CreateSyncObjects()) {
            return false;
        }
        return true;
    }

    void VulkanContext::Clean() {


        for (sizet i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(Device, ImageAvailableSemaphores[i], Allocator);
            vkDestroySemaphore(Device, RenderFinishedSemaphores[i], Allocator);
            vkDestroyFence(Device, InFlightFences[i], Allocator);
        }
        vkDestroyCommandPool(Device, CommandPool, Allocator);

        for(auto buffers: Buffers){
            for(auto buffer: buffers){
                DestroyBuffer(buffer);
            }
        }
        

        CleanupSwapchain();


        /*
        vkDestroyPipeline(Device, GraphicsPipeline, Allocator);
        vkDestroyPipelineLayout(Device, PipelineLayout, Allocator);
        */
        vkDestroyRenderPass(Device, RenderPass, Allocator);

        vkDestroyDevice(Device, Allocator);

        vkDestroySurfaceKHR(Instance, Surface, Allocator);
        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(Instance, DebugMessenger, Allocator);
        }
        vkDestroyInstance(Instance, Allocator);
    }
    bool VulkanContext::SetupDebugMessenger() {

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        PopulateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(Instance, &createInfo, Allocator, &DebugMessenger) != VK_SUCCESS) {
            Logger::Critical("Failed to create VkDebugUtilsMessengerEXT");
            return false;
        }
        return true;
    }

    bool VulkanContext::CreateInstance() {
        if (enableValidationLayers && !CheckValidationLayerSupport()) {
            Logger::Critical("Validation layer requested but not found");
            return false;
        }
        VkApplicationInfo appInfo;
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = AppInfo::Name().c_str();
        appInfo.applicationVersion = 1;
        appInfo.pEngineName = "SylverEngine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;
        appInfo.pNext = nullptr;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = m_Window->GetRequiredExtensions();
        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
#if defined(SYLVER_PLATFORM_MAC)
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
        createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<u32>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            PopulateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }


        if (vkCreateInstance(&createInfo, Allocator, &Instance) != VK_SUCCESS) {
            Logger::Critical("Failed to create VkInstance");
            return false;
        }
        VULKAN_CREATED("VkInstance");
        return true;
    }
    bool VulkanContext::CreateSyncObjects() {
        ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreCreateInfo{};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceCreateInfo{};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (sizet i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(Device, &semaphoreCreateInfo, Allocator, &ImageAvailableSemaphores[i]) != VK_SUCCESS) {
                Logger::Critical("Failed to create VkSemaphore");
                return false;
            }
            VULKAN_CREATED("VkSemaphore");
            if (vkCreateSemaphore(Device, &semaphoreCreateInfo, Allocator, &RenderFinishedSemaphores[i]) != VK_SUCCESS) {
                Logger::Critical("Failed to create VkSemaphore");
                return false;
            }
            VULKAN_CREATED("VkSemaphore");
            if (vkCreateFence(Device, &fenceCreateInfo, Allocator, &InFlightFences[i]) != VK_SUCCESS) {
                Logger::Critical("Failed to create VkFence");
                return false;
            }
            VULKAN_CREATED("VkFence");
        }
        return true;
    }

}    // namespace Sylver
