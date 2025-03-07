#pragma once


namespace Sylver{
    struct Config{
        static Config GetOpts(const std::vector<std::string>& opts);
        enum class eRenderer {
            NONE,
            OPENGL,
            VULKAN
        };
        eRenderer RendererBackend{ eRenderer::NONE };
        u32 RendererWidth{ 0 };
        u32 RendererHeight{ 0 };
    };
}
