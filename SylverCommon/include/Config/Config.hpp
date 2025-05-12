#pragma once


namespace Sylver{
    struct Config{
        static Config GetOpts(const std::vector<std::string>& opts);
        enum class eRenderer {
            NONE,
            SOFTWARE,
            OPENGL,
            VULKAN,
            METAL,
            DIRECT3D,
        };
        struct{
            eRenderer Backend{ eRenderer::NONE };
            u32 Width{ 0 };
            u32 Height{ 0 };
        } Renderer;
    };
}
