#include <Config/Config.hpp>


namespace Sylver {
    enum class eOption {
        NONE,
        RENDERER_BACKEND,

    };
    Config Config::GetOpts(const std::vector<std::string>& opts) {
        Config result{};

        eOption optionToSet = eOption::NONE;
        for (const auto opt : opts) {
            if (optionToSet == eOption::RENDERER_BACKEND) {
                if (opt == "vulkan" || opt == "Vulkan" || opt == "VULKAN") {
                    result.Renderer.Backend = eRenderer::VULKAN;
                }
                if (opt == "opengl" || opt == "OpenGL" || opt == "OPENGL") {
                    result.Renderer.Backend = eRenderer::OPENGL;
                }
            }
            if (opt == "-r" || opt == "--renderer") {
                optionToSet = eOption::RENDERER_BACKEND;
            }
        }

        return result;
    }
}    // namespace Sylver
