#pragma once

#include <Graphics/Renderer.hpp>
#include "OpenGLShader.hpp"
#include <Graphics/Renderer/Camera.hpp>

namespace Sylver {
    class OpenGLRenderer : public Renderer {
        private:
            OpenGLShader* m_TextureShader;
            OpenGLShader* m_SolidShader;
            Camera m_Camera;
        public:
            OpenGLRenderer(u32 width, u32 height, const Config& cfg);
            ~OpenGLRenderer();

            bool BeginFrame() override;
            bool Draw(const VertexArray& vertexArray, const Texture* texture) override;
            bool EndFrame() override;
    };
}    // namespace Sylver
