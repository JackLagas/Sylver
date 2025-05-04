#include "VulkanContext.hpp"
#include "VulkanShared.hpp"

namespace Sylver {

    bool VulkanContext::PickPhysicalDevice() {
        u32 deviceCount{};
        vkEnumeratePhysicalDevices(Instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            Logger::Critical("Failed to Find Suitable Graphics Device!");
            return false;
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(Instance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            if (IsDeviceSuitable(device)) {
                PhysicalDevice = device;
                break;
            }
        }

        if (PhysicalDevice == VK_NULL_HANDLE) {
            Logger::Critical("Failed to Find Suitable Graphics Device!");
            return false;
        }
        return true;
    }
    bool VulkanContext::CreateLogicalDevice() {
        QueueFamilyIndices indices = FindQueueFamilies(PhysicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
        std::set<u32> uniqueQueueFamilies = {
            indices.graphicsFamily.value(),
            indices.presentFamily.value()
        };
        f32 queuePriority = 1.0f;
        for (u32 queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<u32>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<u32>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
        }

        if (vkCreateDevice(PhysicalDevice, &createInfo, Allocator, &Device) != VK_SUCCESS) {
            Logger::Critical("Failed to create VkDevice");
            return false;
        }
        VULKAN_CREATED("VkDevice");
        vkGetDeviceQueue(Device, indices.graphicsFamily.value(), 0, &GraphicsQueue);
        vkGetDeviceQueue(Device, indices.presentFamily.value(), 0, &PresentQueue);
        GraphicsQueueFamily = indices.graphicsFamily.value();


        return true;
    }


}    // namespace Sylver
