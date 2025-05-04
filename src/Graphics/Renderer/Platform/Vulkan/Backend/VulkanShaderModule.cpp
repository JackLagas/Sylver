#include "VulkanShaderModule.hpp"

#include <Core/Logger.hpp>

#include <Shaders/Vulkan/Basic_frag_spv.h>
#include <Shaders/Vulkan/Basic_vert_spv.h>

namespace Sylver {


    VkShaderModule CreateModule(VkDevice device, const u32* code, u32 codeSize, const VkAllocationCallbacks* allocator) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = codeSize;
        createInfo.pCode = code;
        VkShaderModule module;
        if (vkCreateShaderModule(device, &createInfo, allocator, &module) != VK_SUCCESS) {
            Logger::Error("Failed to create VkShaderModule");
            return VK_NULL_HANDLE;
        }
        VULKAN_CREATED("VkShaderModule");
        return module;
    }
    VulkanShaderModule* VulkanShaderModule::Create(VkDevice device, const VkAllocationCallbacks* allocator, eShaderModule module) {
        VulkanShaderModule* shaderModule = new VulkanShaderModule();
        shaderModule->Allocator = allocator;
        shaderModule->m_Device = device;
        switch (module) {
            case eShaderModule::BASIC: {
                VkShaderModule vertexModule = CreateModule(device, BASIC_VERT_SPV, sizeof(BASIC_VERT_SPV), allocator);
                if (vertexModule == VK_NULL_HANDLE) {
                    return nullptr;
                }
                shaderModule->ShaderModules.push_back(vertexModule);
                VkPipelineShaderStageCreateInfo vertexStageInfo{};
                vertexStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vertexStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
                vertexStageInfo.module = vertexModule;
                vertexStageInfo.pName = "main";
                shaderModule->ShaderStages.push_back(vertexStageInfo);

                VkShaderModule fragmentModule = CreateModule(device, BASIC_FRAG_SPV, sizeof(BASIC_FRAG_SPV), allocator);
                if (fragmentModule == VK_NULL_HANDLE) {
                    return nullptr;
                }
                shaderModule->ShaderModules.push_back(fragmentModule);
                VkPipelineShaderStageCreateInfo fragmentStageInfo{};
                fragmentStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                fragmentStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                fragmentStageInfo.module = fragmentModule;
                fragmentStageInfo.pName = "main";
                shaderModule->ShaderStages.push_back(fragmentStageInfo);
            }
        }
        return shaderModule;
    }

}    // namespace Sylver
