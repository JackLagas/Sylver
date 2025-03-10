#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <Defines.hpp>

namespace Sylver {

    struct Vertex {
            glm::vec2 Position{};
            glm::vec4 Color{};

            static VkVertexInputBindingDescription GetBindingDescription() {
                VkVertexInputBindingDescription bindingDescription{};
                bindingDescription.binding = 0;
                bindingDescription.stride = sizeof(Vertex);
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                return bindingDescription;
            }
            static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions() {
                std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions;

                attributeDescriptions[0].binding = 0;
                attributeDescriptions[0].location = 0;
                attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[0].offset = offsetof(Vertex, Position);

                attributeDescriptions[1].binding = 0;
                attributeDescriptions[1].location = 1;
                attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
                attributeDescriptions[1].offset = offsetof(Vertex, Color);

                return attributeDescriptions;
            }
    };
}    // namespace Sylver

