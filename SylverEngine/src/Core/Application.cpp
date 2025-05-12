#include <stb_image.h>

#include "Application.hpp"

#include <IO/Logger.hpp>

#include <IO/Files.hpp>
#include <Config/AppInfo.hpp>


namespace Sylver {
    Application::Application(std::vector<std::string> cmdArgs) {
        Logger::Init(".");

        Files::Init(cmdArgs[0]);

        AppInfo::Init("SylverApp", { 1, 0, 0 });

        Config appConfig = Config::GetOpts(cmdArgs);
        appConfig.Renderer.Width = 1920;
        appConfig.Renderer.Height = 1080;
        if (appConfig.Renderer.Backend == Config::eRenderer::NONE) {
            appConfig.Renderer.Backend = Config::eRenderer::VULKAN;
        }

        m_Window = Window::Create(AppInfo::Name(), appConfig);
        m_Renderer = new Renderer(appConfig, m_Window->GetSurface());


        while (!m_Window->ShouldClose()) {
            m_Window->Update();
        }
    }
    Application::~Application() {
        delete m_Renderer;
    }
}    // namespace Sylver
