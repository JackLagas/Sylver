#ifndef VULKAN_CONTEXT_HPP
#define VULKAN_CONTEXT_HPP

#include <vulkan/vulkan.h>

#include <Graphics/Window.hpp>


namespace Sylver {
    struct QueueFamilyIndices {
            std::optional<u32> graphicsFamily;
            std::optional<u32> presentFamily;

            b8 isComplete() {
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
            b8 FramebufferResized{ false };
            VkRenderPass RenderPass{};
            VkPipelineLayout PipelineLayout{};
            VkPipeline GraphicsPipeline{};
            VkCommandPool CommandPool{};
            std::vector<VkCommandBuffer> CommandBuffers{};
            std::vector<VkSemaphore> ImageAvailableSemaphores{};
            std::vector<VkSemaphore> RenderFinishedSemaphores{};
            std::vector<VkFence> InFlightFences{};
            u32 ImageIndex{ 0 };
            u32 CurrentFrame{ 0 };
            VkBuffer VertexBuffer{};
            VkDeviceMemory VertexBufferMemory{};
            VkBuffer IndexBuffer{};
            VkDeviceMemory IndexBufferMemory{};


            b8 Init(Window* window);
            void Clean();

            void RecreateSwapchain();


        private:
            b8 CreateInstance();
            b8 CheckValidationLayerSupport();
            b8 SetupDebugMessenger();
            QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
            SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);
            VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
            VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
            VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
            b8 CheckDeviceExtensionSupport(VkPhysicalDevice device);
            b8 IsDeviceSuitable(VkPhysicalDevice device);
            b8 PickPhysicalDevice();
            b8 CreateLogicalDevice();
            b8 CreateSwapchain();
            b8 CreateImageViews();
            b8 CreateFramebuffers();
            void CleanupSwapchain();
            b8 CreateRenderPass();
            b8 CreateGraphicsPipeline();
            b8 CreateCommandPool();
            b8 CreateCommandBuffer();
            b8 CreateSyncObjects();
            u32 FindMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties);
            void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
            void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
            b8 CreateVertexBuffer();
            b8 CreateIndexBuffer();
    };
}    // namespace Sylver


#endif
