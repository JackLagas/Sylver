#include "VulkanContext.hpp"

#include <Defines.hpp>
#include <Core/Logger.hpp>

#include "VulkanShared.hpp"

namespace Sylver {
    bool VulkanContext::CreateCommandPool() {
        QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(PhysicalDevice);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(Device, &poolInfo, Allocator, &CommandPool) != VK_SUCCESS) {
            Logger::Critical("Failed to create VkCommandPool");
            return false;
        }
        VULKAN_CREATED("VkCommandPool");

        return true;
    }
    bool VulkanContext::CreateCommandBuffer() {
        CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.commandPool = CommandPool;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = static_cast<u32>(CommandBuffers.size());
        if (vkAllocateCommandBuffers(Device, &allocateInfo, CommandBuffers.data()) != VK_SUCCESS) {
            Logger::Critical("Failed to allocate VkCommandBuffer");
            return false;
        }
        VULKAN_ALLOCATED("VkCommandBuffer");
        return true;
    }
}    // namespace Sylver
