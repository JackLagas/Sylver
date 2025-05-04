#pragma once
#include <Defines.hpp>
#include <Core/Logger.hpp>
#include <vulkan/vulkan.h>


namespace Sylver {
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    const u32 MAX_FRAMES_IN_FLIGHT = 2;
#if defined(NDEBUG)
    constexpr bool enableValidationLayers = false;
#else
    constexpr bool enableValidationLayers = true;
#endif

}    // namespace Sylver
