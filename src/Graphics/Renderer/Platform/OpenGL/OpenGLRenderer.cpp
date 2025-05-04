#include "OpenGLRenderer.hpp"

#include <Core/AppInfo.hpp>
#include <glad/glad.h>
#include <Core/Logger.hpp>

namespace Sylver {

    struct UniformBufferObject{
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    static void FramebufferCallback(u32 width, u32 height, void* ptr){
        glViewport(0, 0, width, height);
    }

    OpenGLRenderer::OpenGLRenderer(u32 width, u32 height, const Config& cfg): m_Camera({0.0f, 0.0f}, {width, height}) {
        m_Window = Window::Create(AppInfo::Name(), width, height, cfg);
        m_Window->LoadGlad();

        glViewport(0, 0, width, height);
        m_Window->SetFramebufferCallback(FramebufferCallback);
        glEnable(GL_DEPTH_TEST);
        m_TextureShader = new OpenGLShader(eShader::TEXTURE);
        m_SolidShader = new OpenGLShader(eShader::SOLID);
        m_TextureShader->Bind();
        m_TextureShader->SetUniformMat4f("proj", m_Camera.GetProjection());
        m_TextureShader->SetUniformMat4f("view", m_Camera.GetView());
        m_TextureShader->SetUniformMat4f("model", glm::mat4(1.0f));
        m_SolidShader->Bind();
        m_SolidShader->SetUniformMat4f("proj", m_Camera.GetProjection());
        m_SolidShader->SetUniformMat4f("view", m_Camera.GetView());
        m_SolidShader->SetUniformMat4f("model", glm::mat4(1.0f));

    }
    OpenGLRenderer::~OpenGLRenderer() {
    }

    bool OpenGLRenderer::BeginFrame() {
        m_Window->Update();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return true;
    }
    bool OpenGLRenderer::EndFrame(){
        return true;
    }
    bool OpenGLRenderer::Draw(const VertexArray& vertexArray, const Texture* texture){
        u32 VAO;
        u32 VBO;
        u32 IBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &IBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(
            GL_ARRAY_BUFFER, 
            Vertex::Stride() * vertexArray.Vertices.size(), 
            vertexArray.Vertices.data(), 
            GL_STATIC_DRAW
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, 
            sizeof(u32) * vertexArray.Indices.size(), 
            vertexArray.Indices.data(), 
            GL_STATIC_DRAW
        );
        // position attribute
        glVertexAttribPointer(
            0, 
            2, 
            GL_FLOAT, 
            GL_FALSE, 
            Vertex::Stride(), 
            (void*)offsetof(Vertex, Position)
        );
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(
            1, 
            2, 
            GL_FLOAT, 
            GL_FALSE, 
            Vertex::Stride(), 
            (void*)offsetof(Vertex, TexCoords)
        );
        glEnableVertexAttribArray(1);
        // color attribute
        glVertexAttribPointer(
                2, 
                4, 
                GL_FLOAT, 
                GL_FALSE, 
                Vertex::Stride(), 
                (void*)offsetof(Vertex, Color)
                );
        glEnableVertexAttribArray(2);

        if(texture){
            m_TextureShader->Bind();
            m_TextureShader->SetUniform1i("texture1", 0);
            texture->Bind();
        }
        else{
            m_SolidShader->Bind();
        }
        glDrawElements(GL_TRIANGLES, vertexArray.Indices.size(), GL_UNSIGNED_INT, nullptr);


        return true;
    }

}    // namespace Sylver
