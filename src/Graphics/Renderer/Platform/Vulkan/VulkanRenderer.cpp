#include "VulkanRenderer.hpp"

#include <Core/Logger.hpp>
#include <Core/AppInfo.hpp>
#include <Core/Files.hpp>
#include <Graphics/Renderer/Camera.hpp>
#include "Backend/VulkanShared.hpp"


namespace Sylver {
    VulkanRenderer::VulkanRenderer(u32 width, u32 height, const Config& cfg) {
        m_Window = Window::Create(AppInfo::Name(), width, height, cfg);
        if (!m_Context.Init(m_Window)) {
            Logger::Critical("Vulkan initialization failed...");
        }
        m_Window->SetUserPtr(&m_Context);
        m_Window->SetFramebufferCallback(SetFramebufferSize);
        m_SolidShader = new VulkanShader(eShader::SOLID, m_Context);
        Camera camera({0, 0}, {1920, 1080}, {-1000.0f, 10000.0f});
        m_MVP.Model = glm::mat4(1.0f);
        m_MVP.Proj = camera.GetProjection();
        m_MVP.View = camera.GetView();
        m_MVP.Proj[1][1] *= -1;
    }

    VulkanRenderer::~VulkanRenderer() {
        Clean();
    }

    void VulkanRenderer::SetFramebufferSize(u32 x, u32 y, void* userPtr) {
        reinterpret_cast<VulkanContext*>(userPtr)->FramebufferResized = true;
        Logger::Info("Framebuffer resized to ({}, {})", x, y);
    }
    bool VulkanRenderer::BeginFrame() {
        m_Window->Update();

        vkWaitForFences(m_Context.Device, 1, &m_Context.InFlightFences[m_Context.CurrentFrame], VK_TRUE, UINT64_MAX);

        VkResult result = vkAcquireNextImageKHR(m_Context.Device, m_Context.Swapchain, UINT64_MAX, m_Context.ImageAvailableSemaphores[m_Context.CurrentFrame], VK_NULL_HANDLE, &m_Context.ImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            m_Context.RecreateSwapchain();
            return false;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            Logger::Error("Failed to acquire swapchain image");
            return false;
        }
        m_SolidShader->SetMVP(m_MVP, m_Context.CurrentFrame);
        vkResetFences(m_Context.Device, 1, &m_Context.InFlightFences[m_Context.CurrentFrame]);
        vkResetCommandBuffer(m_Context.CommandBuffers[m_Context.CurrentFrame], 0);
        for(auto buffer: m_Context.Buffers[m_Context.CurrentFrame]){
            m_Context.DestroyBuffer(buffer);
        }
        m_Context.Buffers[m_Context.CurrentFrame].clear();

        VkCommandBufferBeginInfo commandBufferBeginInfo{};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags = 0;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(m_Context.CommandBuffers[m_Context.CurrentFrame], &commandBufferBeginInfo) != VK_SUCCESS) {
            Logger::Error("Failed to begin recording VkCommandBuffer");
            return false;
        }

        VkRenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = m_SolidShader->GetRenderPass();
        renderPassBeginInfo.framebuffer = m_Context.SwapchainFramebuffers[m_Context.ImageIndex];
        renderPassBeginInfo.renderArea.offset = { 0, 0 };
        renderPassBeginInfo.renderArea.extent = m_Context.SwapchainExtent;
        VkClearValue clearColor = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(m_Context.CommandBuffers[m_Context.CurrentFrame], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(m_Context.CommandBuffers[m_Context.CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_SolidShader->GetPipeline());

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<f32>(m_Context.SwapchainExtent.width);
        viewport.height = static_cast<f32>(m_Context.SwapchainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_Context.CommandBuffers[m_Context.CurrentFrame], 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = m_Context.SwapchainExtent;
        vkCmdSetScissor(m_Context.CommandBuffers[m_Context.CurrentFrame], 0, 1, &scissor);
        


        return true;
    }
    bool VulkanRenderer::Draw(const VertexArray& vertexArray, const Texture* texture){
        VulkanBuffer vertexBuffer = m_Context.CreateVertexBuffer(vertexArray.Vertices);
        VulkanBuffer indexBuffer = m_Context.CreateIndexBuffer(vertexArray.Indices);
        m_Context.Buffers[m_Context.CurrentFrame].push_back(vertexBuffer);
        m_Context.Buffers[m_Context.CurrentFrame].push_back(indexBuffer);

        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(m_Context.CommandBuffers[m_Context.CurrentFrame], 0, 1, &vertexBuffer.Buffer, &offset);
        vkCmdBindIndexBuffer(m_Context.CommandBuffers[m_Context.CurrentFrame], indexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(m_Context.CommandBuffers[m_Context.CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_SolidShader->GetPipelineLayout(), 0, 1, &m_SolidShader->GetDescriptorSets()[m_Context.CurrentFrame], 0, nullptr);
        vkCmdDrawIndexed(m_Context.CommandBuffers[m_Context.CurrentFrame], static_cast<u32>(vertexArray.Indices.size()), 1, 0, 0, 0);

        return true;
    }

    bool VulkanRenderer::EndFrame() {
        VkDeviceSize offset = 0;


        vkCmdEndRenderPass(m_Context.CommandBuffers[m_Context.CurrentFrame]);

        if (vkEndCommandBuffer(m_Context.CommandBuffers[m_Context.CurrentFrame]) != VK_SUCCESS) {
            Logger::Error("Failed to record command buffer!");
            return false;
        }


        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_Context.ImageAvailableSemaphores[m_Context.CurrentFrame];
        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        submitInfo.pWaitDstStageMask = &waitStage;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_Context.CommandBuffers[m_Context.CurrentFrame];
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_Context.RenderFinishedSemaphores[m_Context.CurrentFrame];

        if (vkQueueSubmit(m_Context.GraphicsQueue, 1, &submitInfo, m_Context.InFlightFences[m_Context.CurrentFrame]) != VK_SUCCESS) {
            Logger::Error("Failed to submit draw command buffer");
            return false;
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &m_Context.RenderFinishedSemaphores[m_Context.CurrentFrame];
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &m_Context.Swapchain;
        presentInfo.pImageIndices = &m_Context.ImageIndex;
        presentInfo.pResults = nullptr;

        VkResult result = vkQueuePresentKHR(m_Context.PresentQueue, &presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Context.FramebufferResized) {
            m_Context.FramebufferResized = false;
            m_Context.RecreateSwapchain();
        } else if (result != VK_SUCCESS) {
            Logger::Error("Failed to present to swapchain!");
            return false;
        }
        m_Context.CurrentFrame = (m_Context.CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
        return true;
    }


    void VulkanRenderer::Clean() {
        vkDeviceWaitIdle(m_Context.Device);
        delete m_SolidShader;
        m_Context.Clean();
    }

}    // namespace Sylver
