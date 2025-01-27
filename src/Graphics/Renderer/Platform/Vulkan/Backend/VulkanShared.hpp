#ifndef SYLVER_VULKAN_SHARED_HPP
#define SYLVER_VULKAN_SHARED_HPP

#include <Defines.hpp>
#include <Core/Logger.hpp>
#include <vulkan/vulkan.h>

// TODO: Temporary
#include "../VulkanVertex.hpp"


namespace Sylver {
    // TODO: Temporary
    const std::vector<Vertex> vertices = {
        { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { {  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { {  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } }
    };
    const std::vector<u16> indices = {
        0, 1, 2, 2, 3, 0
    };
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    const u32 MAX_FRAMES_IN_FLIGHT = 2;
#if defined(NDEBUG)
    const b8 enableValidationLayers = false;
#else
    const b8 enableValidationLayers = true;
#endif

}    // namespace Sylver

#endif
