#ifndef SYLVER_VULKAN_CONTEXT_HPP
#define SYLVER_VULKAN_CONTEXT_HPP

#include <vulkan/vulkan.hpp>
#include <Defines.hpp>
#include <Core/AppInfo.hpp>
#include <Core/EngineInfo.hpp>
#include <vulkan/vulkan_handles.hpp>

#include <Graphics/Window.hpp>

#include "VulkanSwapchain.hpp"

namespace Sylver {
    struct QueueFamilyIndices {
            std::optional<u32> GraphicsAndComputeFamily;
            std::optional<u32> PresentFamily;


            b8 IsComplete() {
                return GraphicsAndComputeFamily.has_value() && PresentFamily.has_value();
            }
    };
    struct VulkanContext {

            Window* ActiveWindow;

            vk::Instance Instance{};
            std::vector<vk::PhysicalDevice> PhysicalDevices{};
            vk::PhysicalDevice PhysicalDevice{};
            vk::Device LogicalDevice{};
            vk::SurfaceKHR Surface{};
            VulkanSwapchain Swapchain{};

            vk::DebugUtilsMessengerEXT debugUtilsMessenger{};


            b8 Init(Window* window);
            b8 IsDeviceSuitable(vk::PhysicalDevice device);
            void PickDevice(bool recreateLogicalDevice = true);
            b8 CreateLogicalDevice();
            QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device);
            SwapchainSupportDetails QuerySwapchainSupport(vk::PhysicalDevice device);

            void ChooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
            void ChoosePresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
            void CreateSwapchain();


            void Destroy();
            ~VulkanContext();
    };
}    // namespace Sylver

#endif
