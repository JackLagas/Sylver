#pragma once

#define GLM_FORCE_RADIANS
#include <Graphics/Renderer/Shader.hpp>
#include "Backend/VulkanContext.hpp"
#include "Backend/VulkanBuffer.hpp"
#include <vulkan/vulkan.h>
#include <Defines.hpp>
#include <Graphics/Renderer/MVP.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>

namespace Sylver{
    enum class eShaderModule{
        TEXTURE_FRAG,
        SOLID_FRAG,
        BASIC_VERT,
    };
    class VulkanShader {
        private:
           VkDevice m_Device{}; 
           const VkAllocationCallbacks* m_Allocator{nullptr};
           VkRenderPass m_RenderPass{};
           VkPipelineLayout m_PipelineLayout{};
           VkPipeline m_Pipeline{};
           VkFormat m_SwapchainImageFormat{};
           VkDescriptorSetLayout m_DescriptorSetLayout{};
           VkDescriptorPool m_DescriptorPool{};
           std::vector<VkDescriptorSet> m_DescriptorSets{};
           std::vector<VulkanBuffer> m_UniformBuffers{};
           std::vector<void*> m_UniformBuffersMapped{};
        public:
            VulkanShader(eShader shader, const VulkanContext& ctx);
            ~VulkanShader();

            void Bind() const;
            void Unbind() const;

            bool SetMVP(const MVP& mvp, u32 currentFrame);

            inline VkRenderPass GetRenderPass(){
                return m_RenderPass;
            }
            inline VkPipelineLayout GetPipelineLayout(){
                return m_PipelineLayout;
            }
            inline VkPipeline GetPipeline(){
                return m_Pipeline;
            }
            inline std::vector<VkDescriptorSet>& GetDescriptorSets(){
                return m_DescriptorSets;
            }
            inline void updateUniformBuffer(uint32_t currentImage) {
                static auto startTime = std::chrono::high_resolution_clock::now();

                auto currentTime = std::chrono::high_resolution_clock::now();
                float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

                MVP mvp{};
                mvp.Model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                mvp.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                mvp.Proj = glm::perspective(glm::radians(45.0f), 1920 / (float) 1080, 0.1f, 10.0f);
                mvp.Proj[1][1] *= -1;

                memcpy(m_UniformBuffersMapped[currentImage], &mvp, sizeof(mvp));
            }
        private:
            std::vector<VkPipelineShaderStageCreateInfo> CreateShaderModules(eShader shader);
            bool CreateGraphicsPipeline(const std::vector<VkPipelineShaderStageCreateInfo>& shaderModules);
            bool CreateDescriptorSetLayout(eShader shader);
            bool CreateUniformBuffers(const VulkanContext& ctx);
            bool CreateDescriptorPool();
            bool CreateDescriptorSets();

    };
}
