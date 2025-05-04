#include "VulkanContext.hpp"

#include "VulkanShared.hpp"


namespace Sylver {

    u32 FindMemoryType(VkPhysicalDevice physicalDevice, u32 typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memoryProperties{};
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

        for (u32 i = 0; i < memoryProperties.memoryTypeCount; i++) {
            if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        throw std::runtime_error("Failed to find suitable memory type");
    }

    VulkanBuffer::VulkanBuffer(const VulkanContext& ctx, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties): m_Device(ctx.Device), m_Allocator(ctx.Allocator), Size(size){
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(m_Device, &bufferInfo, m_Allocator, &Buffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create VkBuffer");
        }
        VULKAN_CREATED("VkBuffer");

        VkMemoryRequirements memoryRequirements{};
        vkGetBufferMemoryRequirements(m_Device, Buffer, &memoryRequirements);

        VkMemoryAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = FindMemoryType(ctx.PhysicalDevice, memoryRequirements.memoryTypeBits, properties);
        if (vkAllocateMemory(m_Device, &allocateInfo, m_Allocator, &Memory) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate VkDeviceMemory");
        }
        VULKAN_ALLOCATED("VkDeviceMemory");

        vkBindBufferMemory(m_Device, Buffer, Memory, 0);
    }
    VulkanBuffer::VulkanBuffer(){
        
    }
    VulkanBuffer::~VulkanBuffer(){
    }
    void VulkanBuffer::Init(const VulkanContext& ctx, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties){
        m_Device = ctx.Device;
        m_Allocator = ctx.Allocator;
        Size = size;
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(m_Device, &bufferInfo, m_Allocator, &Buffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create VkBuffer");
        }
        VULKAN_CREATED("VkBuffer");

        VkMemoryRequirements memoryRequirements{};
        vkGetBufferMemoryRequirements(m_Device, Buffer, &memoryRequirements);

        VkMemoryAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = FindMemoryType(ctx.PhysicalDevice, memoryRequirements.memoryTypeBits, properties);
        if (vkAllocateMemory(m_Device, &allocateInfo, m_Allocator, &Memory) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate VkDeviceMemory");
        }
        VULKAN_ALLOCATED("VkDeviceMemory");

        vkBindBufferMemory(m_Device, Buffer, Memory, 0);
    }
    void VulkanBuffer::Destroy(){
        if(m_Device != VK_NULL_HANDLE){
            vkDestroyBuffer(m_Device, Buffer, m_Allocator);
            vkFreeMemory(m_Device, Memory, m_Allocator);
        }
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

    VulkanBuffer VulkanContext::CreateVertexBuffer(const std::vector<Vertex>& vertices){
        VulkanBuffer vertexBuffer;
        try {
            VkDeviceSize bufferSize = vertices.size() * Vertex::Stride();


            VulkanBuffer stagingBuffer{};

            stagingBuffer.Init(
                *this,
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


            void* data;
            vkMapMemory(Device, stagingBuffer.Memory, 0, bufferSize, 0, &data);
            memcpy(data, vertices.data(), (sizet)bufferSize);
            vkUnmapMemory(Device, stagingBuffer.Memory);

            vertexBuffer.Init(
                *this,
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            CopyBuffer(stagingBuffer.Buffer, vertexBuffer.Buffer, bufferSize);
            stagingBuffer.Destroy();
        } catch (std::runtime_error e) {
            Logger::Critical("{}", e.what());
            return VulkanBuffer{};
        }
        return vertexBuffer;
    }
    VulkanBuffer VulkanContext::CreateIndexBuffer(const std::vector<u32>& indices){
        VulkanBuffer indexBuffer;
        try {
            VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();


            VulkanBuffer stagingBuffer{};

            stagingBuffer.Init(
                *this,
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


            void* data;
            vkMapMemory(Device, stagingBuffer.Memory, 0, bufferSize, 0, &data);
            memcpy(data, indices.data(), (sizet)bufferSize);
            vkUnmapMemory(Device, stagingBuffer.Memory);

            indexBuffer.Init(
                *this,
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            CopyBuffer(stagingBuffer.Buffer, indexBuffer.Buffer, bufferSize);
            stagingBuffer.Destroy();
        } catch (std::runtime_error e) {
            Logger::Critical("{}", e.what());
            return VulkanBuffer{};
        }

        return indexBuffer;
    }
    void VulkanContext::DestroyBuffer(VulkanBuffer buffer){
        buffer.Destroy();
    }
}    // namespace Sylver
