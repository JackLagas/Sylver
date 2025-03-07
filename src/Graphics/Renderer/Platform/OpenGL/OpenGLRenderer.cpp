#include "OpenGLRenderer.hpp"

#include <Core/AppInfo.hpp>
#include <glad/glad.h>

namespace Sylver {

    static void FramebufferCallback(u32 width, u32 height, void* ptr){
        glViewport(0, 0, width, height);
    }

    OpenGLRenderer::OpenGLRenderer(u32 width, u32 height, const Config& cfg) {
        m_Window = Window::Create(AppInfo::Name(), width, height, cfg);
        m_Window->LoadGlad();

        glViewport(0, 0, width, height);
        m_Window->SetFramebufferCallback(FramebufferCallback);
    }
    OpenGLRenderer::~OpenGLRenderer() {
    }

    bool OpenGLRenderer::BeginFrame() {
        m_Window->Update();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        return true;
    }
    bool OpenGLRenderer::DrawSprite(glm::vec2 pos, glm::vec2 size, u8* texture) {
        return true;
    }
    bool OpenGLRenderer::DrawSprite(f32 x, f32 y, f32 w, f32 h, u8* texture) {
        return true;
    }
    bool OpenGLRenderer::DrawRect(glm::vec2 pos, glm::vec2 size, glm::vec4 color) {
        return true;
    }
    bool OpenGLRenderer::DrawRect(f32 x, f32 y, f32 w, f32 h, glm::vec4 color) {
        return true;
    }
    bool OpenGLRenderer::EndFrame() {
        return true;
    }

}    // namespace Sylver
