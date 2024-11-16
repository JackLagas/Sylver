#ifndef SYLVER_RENDERER_OBJECT_HPP
#define SYLVER_RENDERER_OBJECT_HPP


#include <Defines.hpp>
#include <glm/glm.hpp>
#include "Texture.hpp"

namespace Sylver {
    struct Object {
            glm::vec2 Position;
            glm::vec2 Size;
            glm::vec4 Color;
            TextureData Texture;
    };

}    // namespace Sylver


#endif
