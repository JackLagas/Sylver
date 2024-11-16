#ifndef SYLVER_VULKAN_RENDERER_HPP
#define SYLVER_VULKAN_RENDERER_HPP

#include <Graphics/Renderer.hpp>

#include <vulkan/vulkan.hpp>

#include "VulkanContext.hpp"

namespace Sylver {
    class VulkanRenderer : public Renderer {
        private:
            VulkanContext m_Context;


        public:
            VulkanRenderer(Window* window);
            ~VulkanRenderer();

            void BeginFrame();
            void Submit(const Object& object);
            void EndFrame();


        private:
            void Clean();
    };
}    // namespace Sylver

#endif
