#include <stb_image.h>

#include "Application.hpp"

#include "Logger.hpp"

#include <Core/Files.hpp>
#include <Core/AppInfo.hpp>

#include <Modules/Module.hpp>

#include <Binary/Writer.hpp>
#include <Binary/Reader.hpp>
#include <Graphics/Renderer/VertexArray.hpp>
#include <Graphics/Renderer/Platform/OpenGL/OpenGLTexture.hpp>
#include <Graphics/Renderer/Camera.hpp>

namespace Sylver {
    Application::Application(std::vector<std::string> cmdArgs) {
        Logger::Init(".");

        Files::Init(cmdArgs[0]);

        AppInfo::Init("SylverApp", { 1, 0, 0 });

        Files::InitUserPath(AppInfo::Name());
        Config appConfig = Config::GetOpts(cmdArgs);
        appConfig.RendererWidth = 1920;
        appConfig.RendererHeight = 1080;
        if (appConfig.RendererBackend == Config::eRenderer::NONE) {
            appConfig.RendererBackend = Config::eRenderer::VULKAN;
        }

        m_Renderer = Renderer::Create(appConfig);


        VertexArray pink = VertexArray::Create({50, 50}, {500, 500}, {1.0f, 0.82f, 0.86f, 1.0f});
        
        stbi_set_flip_vertically_on_load(true);


        while (!m_Renderer->ShouldClose()) {
            if (!m_Renderer->BeginFrame()) {
                continue;
            }
            m_Renderer->Draw(pink, nullptr);

            m_Renderer->EndFrame();
        }
    }
    Application::~Application() {
        delete m_Renderer;
    }
}    // namespace Sylver
