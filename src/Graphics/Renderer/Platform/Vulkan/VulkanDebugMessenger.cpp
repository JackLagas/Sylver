#include "VulkanDebugMessenger.hpp"

#include <sstream>

PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

vk::DebugUtilsMessageSeverityFlagsEXT severityFlags =
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags =
    vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
    vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
    vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;


VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pMessenger) {
    return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, VkAllocationCallbacks const* pAllocator) {
    return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
    void* pUserData) {


    std::string message = fmt::format(
        "Vulkan Error: {}: \n"
        "\tmessageIDName    = {}\n"
        "\tmessage          = <{}>\n",
        vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)),
        pCallbackData->pMessageIdName,
        pCallbackData->pMessage);
    if (pCallbackData->queueLabelCount > 0) {
        message.append("\tQueue Labels: \n");
        for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++) {
            message.append(
                fmt::format(
                    "\t\tlabelName = <{}>\n",
                    pCallbackData->pQueueLabels[i].pLabelName));
        }
    }
    if (pCallbackData->cmdBufLabelCount > 0) {
        message.append("\tCommandBuffer Labels:\n");
        for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
            message.append(
                fmt::format(
                    "\t\tlabelName = <{}>\n",
                    pCallbackData->pCmdBufLabels[i].pLabelName));
        }
    }
    if (pCallbackData->objectCount > 0) {
        message.append("\tObjects:\n");
        for (uint32_t i = 0; i < pCallbackData->objectCount; i++) {
            message.append(
                fmt::format(
                    "\t\tObject {}\n"
                    "\t\t\tobjectType   = {}\n"
                    "\t\t\tobjectHandle = {}\n",
                    i,
                    vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType)),
                    pCallbackData->pObjects[i].objectHandle));
            if (pCallbackData->pObjects[i].pObjectName) {
                message.append(
                    fmt::format(
                        "\t\t\tobjectName   = <{}>\n",
                        pCallbackData->pObjects[i].pObjectName));
            }
        }
    }

    if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
        Sylver::Logger::Info(message);
    } else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        Sylver::Logger::Warn(message);
    } else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        Sylver::Logger::Error(message);
    }

    return true;
}
VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
    void* pUserData) {

    std::stringstream message;

    message << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) << ": "
            << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)) << ":\n";
    message << std::string("\t") << "messageIDName   = <" << pCallbackData->pMessageIdName << ">\n";
    message << std::string("\t") << "messageIdNumber = " << pCallbackData->messageIdNumber << "\n";
    message << std::string("\t") << "message         = <" << pCallbackData->pMessage << ">\n";
    if (0 < pCallbackData->queueLabelCount) {
        message << std::string("\t") << "Queue Labels:\n";
        for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++) {
            message << std::string("\t\t") << "labelName = <" << pCallbackData->pQueueLabels[i].pLabelName << ">\n";
        }
    }
    if (0 < pCallbackData->cmdBufLabelCount) {
        message << std::string("\t") << "CommandBuffer Labels:\n";
        for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
            message << std::string("\t\t") << "labelName = <" << pCallbackData->pCmdBufLabels[i].pLabelName << ">\n";
        }
    }
    if (0 < pCallbackData->objectCount) {
        message << std::string("\t") << "Objects:\n";
        for (uint32_t i = 0; i < pCallbackData->objectCount; i++) {
            message << std::string("\t\t") << "Object " << i << "\n";
            message << std::string("\t\t\t") << "objectType   = " << vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType)) << "\n";
            message << std::string("\t\t\t") << "objectHandle = " << pCallbackData->pObjects[i].objectHandle << "\n";
            if (pCallbackData->pObjects[i].pObjectName) {
                message << std::string("\t\t\t") << "objectName   = <" << pCallbackData->pObjects[i].pObjectName << ">\n";
            }
        }
    }
    Sylver::Logger::Warn(message.str());
    return false;
}
namespace Sylver {

    vk::DebugUtilsMessengerEXT CreateDebugMessenger(vk::Instance instance) {
        pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
        if (!pfnVkCreateDebugUtilsMessengerEXT) {
            Logger::Error("GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function.");
        }

        pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
        if (!pfnVkDestroyDebugUtilsMessengerEXT) {
            Logger::Error("GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function.");
        }

        return instance.createDebugUtilsMessengerEXT(
            vk::DebugUtilsMessengerCreateInfoEXT(
                {},
                severityFlags,
                messageTypeFlags,
                &debugMessageCallback));
    }
}    // namespace Sylver
