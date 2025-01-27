#include "Application.hpp"

#include "Logger.hpp"

#include <Core/Files.hpp>
#include <Core/AppInfo.hpp>

#include <Modules/Module.hpp>

#include <Binary/Writer.hpp>
#include <Binary/Reader.hpp>

namespace Sylver {
    Application::Application(std::vector<std::string> cmdArgs) {
        Logger::Init(".");

        Files::Init(cmdArgs[0]);

        AppInfo::Init("SylverApp", { 1, 0, 0 });

        Files::InitUserPath(AppInfo::Name());

        m_Renderer = Renderer::Create({ Settings::sRenderer::RendererType::VULKAN, 1920, 1080 });


        while (!m_Renderer->ShouldClose()) {
            if (!m_Renderer->BeginFrame()) {
                continue;
            }
            m_Renderer->DrawRect({ -0.9f, -0.9f }, { 1.8f, 1.8f }, { 0.8f, 0.2f, 0.6f, 1.0f });

            m_Renderer->EndFrame();
        }
    }
    Application::~Application() {
        delete m_Renderer;
    }
}    // namespace Sylver
