#pragma once

#include <vulkan/vulkan.h>

#include <Graphics/Window.hpp>

#include <Graphics/Renderer/VertexArray.hpp>
#include "VulkanBuffer.hpp"


namespace Sylver {
    struct QueueFamilyIndices {
            std::optional<u32> graphicsFamily;
            std::optional<u32> presentFamily;

            bool isComplete() {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
    };
    struct SwapchainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities{};
            std::vector<VkSurfaceFormatKHR> formats{};
            std::vector<VkPresentModeKHR> presentModes{};
    };
    struct VulkanContext {
        private:
            Window* m_Window{ nullptr };

        public:
            VkAllocationCallbacks* Allocator{ nullptr };
            VkInstance Instance{};
            VkDebugUtilsMessengerEXT DebugMessenger{};
            VkPhysicalDevice PhysicalDevice{};
            VkDevice Device{};
            VkQueue GraphicsQueue{};
            VkQueue PresentQueue{};
            VkSurfaceKHR Surface{};
            VkSwapchainKHR Swapchain{};
            VkFormat SwapchainImageFormat{};
            VkExtent2D SwapchainExtent{};
            std::vector<VkImage> SwapchainImages{};
            std::vector<VkImageView> SwapchainImageViews{};
            std::vector<VkFramebuffer> SwapchainFramebuffers{};
            bool FramebufferResized{ false };
            VkRenderPass RenderPass{};
            /*
            VkPipelineLayout PipelineLayout{};
            VkPipeline GraphicsPipeline{};
            */
            VkCommandPool CommandPool{};
            std::vector<VkCommandBuffer> CommandBuffers{};
            std::vector<std::vector<VulkanBuffer>> Buffers;
            std::vector<VkSemaphore> ImageAvailableSemaphores{};
            std::vector<VkSemaphore> RenderFinishedSemaphores{};
            std::vector<VkFence> InFlightFences{};
            u32 ImageIndex{ 0 };
            u32 CurrentFrame{ 0 };
            u32 ImageCount{0};
            u32 GraphicsQueueFamily{0};


            bool Init(Window* window);
            void Clean();

            void RecreateSwapchain();

            VulkanBuffer CreateVertexBuffer(const std::vector<Vertex>& vertices);
            VulkanBuffer CreateIndexBuffer(const std::vector<u32>& indices);

            void DestroyBuffer(VulkanBuffer buffer);


        private:
            bool CreateInstance();
            bool CheckValidationLayerSupport();
            bool SetupDebugMessenger();
            QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
            SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);
            VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
            VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
            VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
            bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
            bool IsDeviceSuitable(VkPhysicalDevice device);
            bool PickPhysicalDevice();
            bool CreateLogicalDevice();
            bool CreateSwapchain();
            bool CreateImageViews();
            bool CreateFramebuffers();
            void CleanupSwapchain();
            bool CreateRenderPass();
            /*
            bool CreateGraphicsPipeline();
            */
            bool CreateCommandPool();
            bool CreateCommandBuffer();
            bool CreateSyncObjects();
            void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
            void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    };
}    // namespace Sylver


