#pragma once
#include <Graphics/Renderer.hpp>

namespace Sylver {
    class Application {
        private:
            Renderer* m_Renderer{ nullptr };

        public:
            Application(std::vector<std::string> cmdArgs);
            ~Application();
    };
}    // namespace Sylver
