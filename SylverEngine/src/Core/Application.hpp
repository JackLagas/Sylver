#pragma once
#include <Renderer/Renderer.hpp>
#include <Window/Window.hpp>

namespace Sylver {
    class Application {
        private:
            Renderer* m_Renderer{ nullptr };
            Window* m_Window{nullptr};

        public:
            Application(std::vector<std::string> cmdArgs);
            ~Application();
    };
}    // namespace Sylver
