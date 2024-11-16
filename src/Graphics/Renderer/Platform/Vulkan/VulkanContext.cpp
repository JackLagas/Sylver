#include "VulkanContext.hpp"

#include "VulkanDebugMessenger.hpp"

#include <limits>
#include <algorithm>


namespace Sylver {


    std::vector<std::string> required_device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    std::vector<std::string> validation_layers = {
        "VK_LAYER_KHRONOS_validation"
    };

    b8 VulkanContext::Init(Window* window) {
        ActiveWindow = window;
        auto extensions = ActiveWindow->GetRequiredExtensions();
        for (auto extension : extensions) {
            Logger::Info(extension);
        }
        std::vector<vk::ExtensionProperties> props = vk::enumerateInstanceExtensionProperties();

        auto propertyIterator = std::find_if(
            props.begin(),
            props.end(),
            [](vk::ExtensionProperties const& ep) { return strcmp(ep.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0; });
        if (propertyIterator == props.end()) {
            Logger::Critical("Something went very wrong, cannot find {} extension", VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            return false;
        }
        std::vector<const char*> layers;
#if !defined(NDEBUG)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        for (const auto& layer : validation_layers) {
            layers.push_back(layer.c_str());
        }
#endif
        vk::ApplicationInfo appInfo(
            AppInfo::Name().c_str(),
            AppInfo::Version().vkVersion(),
            EngineInfo::Name().c_str(),
            EngineInfo::Version().vkVersion(),
            VK_API_VERSION_1_3);
        vk::InstanceCreateInfo instanceCreateInfo(
            vk::InstanceCreateFlags(),
            &appInfo,
            layers,
            extensions,
            nullptr);
        Instance = vk::createInstance(instanceCreateInfo);
        Logger::Info("Successfully created Vulkan Instance");

#if !defined(NDEBUG)
        debugUtilsMessenger = CreateDebugMessenger(Instance);
#endif

        vk::Result surfaceResult = ActiveWindow->CreateWindowSurface(Instance, nullptr, &Surface);
        if (surfaceResult != vk::Result::eSuccess) {
            Logger::Info("Failed to create surface with error {}", vk::to_string(surfaceResult));
        }

        PhysicalDevices = Instance.enumeratePhysicalDevices();

        PickDevice();


        return true;
    }

    b8 VulkanContext::IsDeviceSuitable(vk::PhysicalDevice device) {
        QueueFamilyIndices indices = FindQueueFamilies(PhysicalDevice);

        std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();

        std::set<std::string> requiredExtensions(required_device_extensions.begin(), required_device_extensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        b8 extensionsSupported = requiredExtensions.empty();

        b8 swapchainAdequate = false;
        if (extensionsSupported) {
            SwapchainSupportDetails swapchainSupport = QuerySwapchainSupport(device);
            swapchainAdequate = !swapchainSupport.Formats.empty() && !swapchainSupport.PresentModes.empty();
        }
        return indices.IsComplete() && extensionsSupported && swapchainAdequate;
    }

    void VulkanContext::PickDevice(bool recreateLogicalDevice) {
        vk::PhysicalDevice selectedDevice = PhysicalDevices.front();

        for (auto device : PhysicalDevices) {
            if (device.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu && IsDeviceSuitable(device)) {
                selectedDevice = device;
                break;
            }
        }
        PhysicalDevice = selectedDevice;
        if (recreateLogicalDevice) {
            CreateLogicalDevice();
        }
    }
    QueueFamilyIndices VulkanContext::FindQueueFamilies(vk::PhysicalDevice device) {
        QueueFamilyIndices indices;
        std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

        usize i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if ((queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) && (queueFamily.queueFlags & vk::QueueFlagBits::eCompute)) {
                indices.GraphicsAndComputeFamily = i;
            }
            vk::Bool32 presentSupport = device.getSurfaceSupportKHR(i, Surface);

            if (presentSupport) {
                indices.PresentFamily = i;
            }
            if (indices.IsComplete()) {
                break;
            }
            i++;
        }
        return indices;
    }
    SwapchainSupportDetails VulkanContext::QuerySwapchainSupport(vk::PhysicalDevice device) {
        SwapchainSupportDetails details;
        details.Capabilities = device.getSurfaceCapabilitiesKHR(Surface);
        details.Formats = device.getSurfaceFormatsKHR(Surface);
        details.PresentModes = device.getSurfacePresentModesKHR(Surface);
        return details;
    }

    b8 VulkanContext::CreateLogicalDevice() {
        QueueFamilyIndices indices = FindQueueFamilies(PhysicalDevice);

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        std::set<u32> uniqueQueueFamilies = { indices.GraphicsAndComputeFamily.value(), indices.PresentFamily.value() };

        f32 queuePriority = 1.0f;
        for (u32 queueFamily : uniqueQueueFamilies) {
            vk::DeviceQueueCreateInfo createInfo({}, queueFamily, 1, &queuePriority, nullptr);
            queueCreateInfos.push_back(createInfo);
        }

        std::vector<const char*> layers;
#if !defined(NDEBUG)
        for (const auto& layer : validation_layers) {
            layers.push_back(layer.c_str());
        }
#endif
        std::vector<const char*> extensions;
        for (const auto& extension : required_device_extensions) {
            extensions.push_back(extension.c_str());
        }


        vk::PhysicalDeviceFeatures deviceFeatures;
        vk::DeviceCreateInfo createInfo(
            {},
            queueCreateInfos,
            {},
            extensions,
            &deviceFeatures,
            nullptr);
        vk::Result deviceResult = PhysicalDevice.createDevice(&createInfo, nullptr, &LogicalDevice);
        if (deviceResult != vk::Result::eSuccess) {
            Logger::Error("Failed to create Vulkan Device! {}", vk::to_string(deviceResult));
            return false;
        }

        Logger::Info("Successfully created Vulkan Device!");
        return true;
    }

    void VulkanContext::CreateSwapchain() {
        SwapchainSupportDetails details = QuerySwapchainSupport(PhysicalDevice);
        Swapchain.Init(details, Surface, ActiveWindow);
    }


    void VulkanContext::Destroy() {
        LogicalDevice.destroy();
        Instance.destroySurfaceKHR(Surface);
#if !defined(NDEBUG)
        Instance.destroyDebugUtilsMessengerEXT(debugUtilsMessenger);
#endif
        Instance.destroy();
    }

    VulkanContext::~VulkanContext() {
        Destroy();
    }
}    // namespace Sylver
