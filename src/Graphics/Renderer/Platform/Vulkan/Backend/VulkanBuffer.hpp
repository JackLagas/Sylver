#pragma once

#include <Defines.hpp>
#include <vulkan/vulkan.h>

namespace Sylver{
    struct VulkanContext;
    struct VulkanBuffer{
        private:
            VkDevice m_Device{};
            const VkAllocationCallbacks* m_Allocator{nullptr};
        public:
            VkBuffer Buffer;
            VkDeviceMemory Memory;
            VkDeviceSize Size;

            VulkanBuffer();
            VulkanBuffer(const VulkanContext& ctx, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
            ~VulkanBuffer();
            void Init(const VulkanContext& ctx, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
            void Destroy();
    };
}
