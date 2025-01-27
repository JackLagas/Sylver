#include "VulkanContext.hpp"

#include "VulkanShared.hpp"


namespace Sylver {

    void VulkanContext::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(Device, &bufferInfo, Allocator, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create VkBuffer");
        }
        VULKAN_CREATED("VkBuffer");

        VkMemoryRequirements memoryRequirements{};
        vkGetBufferMemoryRequirements(Device, buffer, &memoryRequirements);

        VkMemoryAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties);
        if (vkAllocateMemory(Device, &allocateInfo, Allocator, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate VkDeviceMemory");
        }
        VULKAN_ALLOCATED("VkDeviceMemory");

        vkBindBufferMemory(Device, buffer, bufferMemory, 0);
    }
    void VulkanContext::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = CommandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(Device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(GraphicsQueue);

        vkFreeCommandBuffers(Device, CommandPool, 1, &commandBuffer);
    }
    u32 VulkanContext::FindMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memoryProperties{};
        vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &memoryProperties);

        for (u32 i = 0; i < memoryProperties.memoryTypeCount; i++) {
            if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        throw std::runtime_error("Failed to find suitable memory type");
    }

    b8 VulkanContext::CreateVertexBuffer() {
        try {
            VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();


            VkBuffer stagingBuffer{};
            VkDeviceMemory stagingBufferMemory{};

            CreateBuffer(
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer,
                stagingBufferMemory);


            void* data;
            vkMapMemory(Device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, vertices.data(), (usize)bufferSize);
            vkUnmapMemory(Device, stagingBufferMemory);

            CreateBuffer(
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                VertexBuffer,
                VertexBufferMemory);
            CopyBuffer(stagingBuffer, VertexBuffer, bufferSize);
            vkDestroyBuffer(Device, stagingBuffer, Allocator);
            vkFreeMemory(Device, stagingBufferMemory, Allocator);
        } catch (std::runtime_error e) {
            Logger::Critical("{}", e.what());
            return false;
        }

        return true;
    }
    b8 VulkanContext::CreateIndexBuffer() {
        try {
            VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();


            VkBuffer stagingBuffer{};
            VkDeviceMemory stagingBufferMemory{};

            CreateBuffer(
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer,
                stagingBufferMemory);


            void* data;
            vkMapMemory(Device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, indices.data(), (usize)bufferSize);
            vkUnmapMemory(Device, stagingBufferMemory);

            CreateBuffer(
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                IndexBuffer,
                IndexBufferMemory);
            CopyBuffer(stagingBuffer, IndexBuffer, bufferSize);
            vkDestroyBuffer(Device, stagingBuffer, Allocator);
            vkFreeMemory(Device, stagingBufferMemory, Allocator);
        } catch (std::runtime_error e) {
            Logger::Critical("{}", e.what());
            return false;
        }

        return true;

        return true;
    }
}    // namespace Sylver
