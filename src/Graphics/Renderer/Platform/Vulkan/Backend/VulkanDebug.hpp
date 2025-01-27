#ifndef SYLVER_VULKAN_DEBUG_MESSENGER_HPP
#define SYLVER_VULKAN_DEBUG_MESSENGER_HPP

#include <vulkan/vulkan.h>
#include <Core/Logger.hpp>

namespace Sylver {
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    // Debug Allocations
    void* VulkanAllocation(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
    void* VulkanReallocation(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
    void VulkanFree(void* pUserData, void* pMemory);

}    // namespace Sylver

#endif
