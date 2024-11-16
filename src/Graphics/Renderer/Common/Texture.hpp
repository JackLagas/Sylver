#ifndef SYLVER_RENDERER_TEXTURE_HPP
#define SYLVER_RENDERER_TEXTURE_HPP

#include <Defines.hpp>
#include <glm/glm.hpp>

namespace Sylver {
    struct TextureData {
            std::string Name{ "" };
            glm::uvec2 Tile{ 0, 0 };
    };
}    // namespace Sylver

#endif
