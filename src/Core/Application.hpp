#ifndef SYLVER_APPLICATION_HPP
#define SYLVER_APPLICATION_HPP

#include <Graphics/Window.hpp>
#include <Graphics/Renderer.hpp>

namespace Sylver {
    class Application {
        private:
            Window* m_MainWindow{ nullptr };
            Renderer* m_Renderer{ nullptr };

        public:
            Application(std::vector<std::string> cmdArgs);
            ~Application();
    };
}    // namespace Sylver

#endif
