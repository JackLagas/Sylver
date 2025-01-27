#ifndef SYLVER_VULKAN_SHADER_HPP
#define SYLVER_VULKAN_SHADER_HPP

#include <Defines.hpp>
#include <vulkan/vulkan.h>

namespace Sylver {
    /**
     * @brief Holds the data for a Vulkan Shader
     *
     */
    struct VulkanShaderModule {
            enum class eShaderModule {
                BASIC
            };
            const VkAllocationCallbacks* Allocator{ nullptr };
            VkDevice m_Device{};
            std::vector<VkPipelineShaderStageCreateInfo> ShaderStages{};
            std::vector<VkShaderModule> ShaderModules{};
            static VulkanShaderModule* Create(VkDevice device, const VkAllocationCallbacks* allocator, eShaderModule module);
            ~VulkanShaderModule() {
                for (auto module : ShaderModules) {
                    vkDestroyShaderModule(m_Device, module, Allocator);
                }
            }
    };
}    // namespace Sylver

#endif
