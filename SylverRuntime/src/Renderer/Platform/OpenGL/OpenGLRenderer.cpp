#include "OpenGLRenderer.hpp"

#include <Config/AppInfo.hpp>
#include <glad/glad.h>
#include <IO/Logger.hpp>

namespace Sylver {

    struct UniformBufferObject{
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    OpenGLRenderer::OpenGLRenderer(const Config& cfg, RenderSurface* surface) {

    }
    OpenGLRenderer::~OpenGLRenderer() {
    }

    bool OpenGLRenderer::BeginFrame() {
        return true;
    }
    bool OpenGLRenderer::EndFrame(){
        return true;
    }
    bool OpenGLRenderer::Draw(const VertexArray& vertArray){
        return true;
    }

}    // namespace Sylver
