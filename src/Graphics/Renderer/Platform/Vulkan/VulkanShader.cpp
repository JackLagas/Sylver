#include "VulkanShader.hpp"

#include <Core/Logger.hpp>
#include <Graphics/Renderer/Vertex.hpp>
#include "Backend/VulkanShared.hpp"

#include <Shaders/Vulkan/Main_vert_spv.h>
#include <Shaders/Vulkan/Solid_frag_spv.h>
#include <Shaders/Vulkan/Texture_frag_spv.h>


namespace Sylver{
    static VkVertexInputBindingDescription GetBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = Vertex::Stride();
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }
    static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions;

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, Position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, TexCoords);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, Color);

        return attributeDescriptions;
    }

    VulkanShader::VulkanShader(eShader shader, const VulkanContext& ctx): m_Device(ctx.Device), m_Allocator(ctx.Allocator), m_SwapchainImageFormat(ctx.SwapchainImageFormat){
        m_RenderPass = ctx.RenderPass;
        CreateDescriptorSetLayout(shader);
        std::vector<VkPipelineShaderStageCreateInfo> shaderModules = CreateShaderModules(shader);;
        CreateGraphicsPipeline(shaderModules);
        for(auto shaderStage : shaderModules){
            vkDestroyShaderModule(m_Device, shaderStage.module, m_Allocator);
        }
        CreateUniformBuffers(ctx);
        CreateDescriptorPool();
        CreateDescriptorSets();
    }
    VulkanShader::~VulkanShader(){
        vkDestroyPipeline(m_Device, m_Pipeline, m_Allocator);
        vkDestroyPipelineLayout(m_Device, m_PipelineLayout, m_Allocator);
        for(sizet i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
            m_UniformBuffers[i].Destroy();
        }
        vkDestroyDescriptorPool(m_Device, m_DescriptorPool, m_Allocator);
        vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, m_Allocator);
    }
    std::vector<VkPipelineShaderStageCreateInfo> VulkanShader::CreateShaderModules(eShader shader){
        std::vector<VkPipelineShaderStageCreateInfo> shaderModules;
        switch(shader){
            case eShader::TEXTURE:{
                VkShaderModuleCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                shaderModules.resize(2); 
                createInfo.codeSize = sizeof(TEXTURE_FRAG_SPV);
                createInfo.pCode = TEXTURE_FRAG_SPV;
                if(vkCreateShaderModule(m_Device, &createInfo, m_Allocator, &shaderModules[0].module) != VK_SUCCESS){
                    Logger::Error("Failed to creat VkShaderModule!");
                    return {};
                }
                VULKAN_CREATED("VkShaderModule");
                shaderModules[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                shaderModules[0].pName = "main";
                shaderModules[0].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                createInfo.codeSize = sizeof(MAIN_VERT_SPV);
                createInfo.pCode = MAIN_VERT_SPV;
                if(vkCreateShaderModule(m_Device, &createInfo, m_Allocator, &shaderModules[1].module) != VK_SUCCESS){
                    Logger::Error("Failed to creat VkShaderModule!");
                    return {};
                }
                VULKAN_CREATED("VkShaderModule");
                shaderModules[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                shaderModules[1].pName = "main";
                shaderModules[1].stage = VK_SHADER_STAGE_VERTEX_BIT;
            }break;
            case eShader::SOLID:{
                VkShaderModuleCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                shaderModules.resize(2); 
                createInfo.codeSize = sizeof(SOLID_FRAG_SPV);
                createInfo.pCode = SOLID_FRAG_SPV;
                if(vkCreateShaderModule(m_Device, &createInfo, m_Allocator, &shaderModules[0].module) != VK_SUCCESS){
                    Logger::Error("Failed to creat VkShaderModule!");
                    return {};
                }
                VULKAN_CREATED("VkShaderModule");
                shaderModules[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                shaderModules[0].pName = "main";
                shaderModules[0].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                createInfo.codeSize = sizeof(MAIN_VERT_SPV);
                createInfo.pCode = MAIN_VERT_SPV;
                if(vkCreateShaderModule(m_Device, &createInfo, m_Allocator, &shaderModules[1].module) != VK_SUCCESS){
                    Logger::Error("Failed to creat VkShaderModule!");
                    return {};
                }
                VULKAN_CREATED("VkShaderModule");
                shaderModules[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                shaderModules[1].pName = "main";
                shaderModules[1].stage = VK_SHADER_STAGE_VERTEX_BIT;
            }break;

        }
        return shaderModules;
    }
    bool VulkanShader::CreateGraphicsPipeline(const std::vector<VkPipelineShaderStageCreateInfo>& shaderModules){
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        auto bindingDescription = GetBindingDescription();
        auto attributeDescriptions = GetAttributeDescriptions();
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<u32>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
        inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportStateInfo{};
        viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportStateInfo.viewportCount = 1;
        viewportStateInfo.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
        rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizerInfo.depthClampEnable = VK_FALSE;
        rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizerInfo.lineWidth = 1.0f;
        rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizerInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizerInfo.depthBiasEnable = VK_FALSE;
        rasterizerInfo.depthBiasConstantFactor = 0.0f;
        rasterizerInfo.depthBiasClamp = 0.0f;
        rasterizerInfo.depthBiasSlopeFactor = 0.0f;

        VkPipelineMultisampleStateCreateInfo multisamplingInfo{};
        multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisamplingInfo.sampleShadingEnable = VK_FALSE;
        multisamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisamplingInfo.minSampleShading = 1.0f;
        multisamplingInfo.pSampleMask = nullptr;
        multisamplingInfo.alphaToCoverageEnable = VK_FALSE;
        multisamplingInfo.alphaToOneEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                                              VK_COLOR_COMPONENT_G_BIT |
                                              VK_COLOR_COMPONENT_B_BIT |
                                              VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
        colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendInfo.logicOpEnable = VK_FALSE;
        colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
        colorBlendInfo.attachmentCount = 1;
        colorBlendInfo.pAttachments = &colorBlendAttachment;
        colorBlendInfo.blendConstants[0] = 0.0f;
        colorBlendInfo.blendConstants[1] = 0.0f;
        colorBlendInfo.blendConstants[2] = 0.0f;
        colorBlendInfo.blendConstants[3] = 0.0f;


        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };
        VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
        dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicStateInfo.pDynamicStates = dynamicStates.data();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;

        if (vkCreatePipelineLayout(m_Device, &pipelineLayoutInfo, m_Allocator, &m_PipelineLayout) != VK_SUCCESS) {
            Logger::Critical("Failed to create VkPipelineLayout");
            return false;
        }
        VULKAN_CREATED("VkPipelineLayout");

        VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
        pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.stageCount = static_cast<u32>(shaderModules.size());
        pipelineCreateInfo.pStages = shaderModules.data();
        pipelineCreateInfo.pVertexInputState = &vertexInputInfo;
        pipelineCreateInfo.pInputAssemblyState = &inputAssemblyInfo;
        pipelineCreateInfo.pViewportState = &viewportStateInfo;
        pipelineCreateInfo.pRasterizationState = &rasterizerInfo;
        pipelineCreateInfo.pMultisampleState = &multisamplingInfo;
        pipelineCreateInfo.pDepthStencilState = nullptr;
        pipelineCreateInfo.pColorBlendState = &colorBlendInfo;
        pipelineCreateInfo.pDynamicState = &dynamicStateInfo;
        pipelineCreateInfo.layout = m_PipelineLayout;
        pipelineCreateInfo.renderPass = m_RenderPass;
        pipelineCreateInfo.subpass = 0;
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, m_Allocator, &m_Pipeline) != VK_SUCCESS) {
            Logger::Critical("Failed to create VkPipeline");
            return false;
        }
        VULKAN_CREATED("VkPipeline");
        return true;
    }
    bool VulkanShader::CreateDescriptorSetLayout(eShader shader){
        VkDescriptorSetLayoutBinding uboLayoutBinding;
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        if(vkCreateDescriptorSetLayout(m_Device, &layoutInfo, m_Allocator, &m_DescriptorSetLayout) != VK_SUCCESS){
            Logger::Error("Failed to create VkDescriptorSetLayout");
            return false;
        }
        VULKAN_CREATED("VkDescriptorSetLayout");
        
        return true;
    }
    bool VulkanShader::CreateUniformBuffers(const VulkanContext& ctx){
        VkDeviceSize bufferSize = sizeof(MVP);
        m_UniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        m_UniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);
        for(sizet i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
            m_UniformBuffers[i].Init(
                ctx, 
                bufferSize, 
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );
            vkMapMemory(m_Device, m_UniformBuffers[i].Memory, 0, bufferSize, 0, &m_UniformBuffersMapped[i]);
        }
        return true;
    }
    bool VulkanShader::CreateDescriptorPool(){
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = static_cast<u32>(MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPoolCreateInfo poolCreateInfo{};
        poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolCreateInfo.poolSizeCount = 1;
        poolCreateInfo.pPoolSizes = &poolSize;
        poolCreateInfo.maxSets = static_cast<u32>(MAX_FRAMES_IN_FLIGHT);

        if(vkCreateDescriptorPool(m_Device, &poolCreateInfo, m_Allocator, &m_DescriptorPool) != VK_SUCCESS){
            Logger::Error("Failed to create VkDescriptorPool");
            return false;
        }
        return true;
    }
    bool VulkanShader::CreateDescriptorSets(){
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_DescriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_DescriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        m_DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

        if(vkAllocateDescriptorSets(m_Device, &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS){
            Logger::Error("Failed to allocate VkDescriptorSets");
            return false;
        }

        for(sizet i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = m_UniformBuffers[i].Buffer;
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(MVP);
            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = m_DescriptorSets[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;
            descriptorWrite.pImageInfo = nullptr; // Optional
            descriptorWrite.pTexelBufferView = nullptr; // Optional

            vkUpdateDescriptorSets(m_Device, 1, &descriptorWrite, 0, nullptr);
        }
        return true;
    }
    void VulkanShader::Bind() const{

    }
    void VulkanShader::Unbind() const{

    }
    bool VulkanShader::SetMVP(const MVP& mvp, u32 currentFrame)  {
        memcpy(m_UniformBuffersMapped[currentFrame], &mvp, sizeof(mvp)); 
        return true;
    }
}




