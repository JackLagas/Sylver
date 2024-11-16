#ifndef SYLVER_SETTINGS_HPP
#define SYLVER_SETTINGS_HPP

#include <Defines.hpp>

namespace Sylver {
    struct Settings {
            struct sRenderer {
                    enum class RendererType {
                        NONE,
                        OPENGL,
                        VULKAN
                    };
                    RendererType Backend{ RendererType::NONE };
                    u32 RendererWidth{ 0 };
                    u32 RendererHeight{ 0 };
            };
    };
}    // namespace Sylver

#endif
