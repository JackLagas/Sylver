#ifndef SYLVER_VULKAN_DEBUG_MESSENGER_HPP
#define SYLVER_VULKAN_DEBUG_MESSENGER_HPP

#include <vulkan/vulkan.hpp>
#include <Defines.hpp>
#include <Core/Logger.hpp>


namespace Sylver {

    vk::DebugUtilsMessengerEXT CreateDebugMessenger(vk::Instance instance);
}    // namespace Sylver


#endif
