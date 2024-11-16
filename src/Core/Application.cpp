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

        Logger::Info(Files::ExecutablePath().c_str());

        AppInfo::Init("SylverApp", { 1, 0, 0 });

        Files::InitUserPath(AppInfo::Name());

        Logger::Info("User path: {}", Files::UserPath().string());

        std::filesystem::remove(Files::RelativeApplication("SimpleScene.slvb"));
        BinaryWriter writer;
        writer.SetFormatVersion(1, 0);
        writer.SetModuleVersion(1, 0, 0);
        writer.AddTexture("Sylveon", Files::RelativeApplication("SimpleScene/assets/textures/Sylveon.jpg"), 0, 0);
        writer.Write(Files::RelativeApplication("SimpleScene.slvb"));

        BinaryReader reader;
        reader.Read(Files::RelativeApplication("SimpleScene.slvb"));
        Logger::Info(reader.Get().ToString());


        auto simpleScene = Module::Load("SimpleScene");
        if (simpleScene.has_value()) {
            Logger::Info("Module Name: {}", simpleScene.value().name);
            Logger::Info("Module Version: {}", simpleScene.value().version.ToString());
            for (auto& [key, path] : simpleScene.value().textures) {
                Logger::Info("Module texture {}: {}", key, path.string());
            }
            for (auto& [key, path] : simpleScene.value().scripts) {
                Logger::Info("Module script {}: {}", key, path.string());
            }
            for (auto& [key, path] : simpleScene.value().shaders) {
                Logger::Info("Module shader {}: {}", key, path.string());
            }
        }

        m_MainWindow = Window::Create("Sylver", 1920, 1080);
        m_Renderer = Renderer::Create(m_MainWindow);

        Logger::Info("Renderer Initialized");


        while (!m_MainWindow->ShouldClose()) {
            m_MainWindow->Update();
        }
    }
    Application::~Application() {
        delete m_Renderer;
        delete m_MainWindow;
    }
}    // namespace Sylver
