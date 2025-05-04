#include "OpenGLTexture.hpp"

#include <glad/glad.h>
#include <Core/Logger.hpp>
#include <stb_image.h>
#include <fstream>

namespace Sylver{
    OpenGLTexture::OpenGLTexture(const std::filesystem::path& path): m_FilePath(path), m_Size(std::filesystem::file_size(path)){
        LoadCPU(); 
    }
    OpenGLTexture::OpenGLTexture(const std::filesystem::path& path, const u32 offset, const u32 size): 
        m_FilePath(path), m_Offset(offset), m_Size(size){
        LoadCPU(); 
    }
    OpenGLTexture::~OpenGLTexture(){
        if(InCPU()) FreeCPU();
        if(InGPU()) FreeGPU();
    }

    bool OpenGLTexture::InCPU() const {
        return m_Data != nullptr;
    }
    bool OpenGLTexture::InGPU() const {
        return m_ID != 0;
    }

    void OpenGLTexture::LoadCPU(){
        u8* data = (u8*)malloc(m_Size);
        std::ifstream file(m_FilePath, std::ios::in | std::ios::binary | std::ios::ate);
        file.seekg(m_Offset);
        file.read((char*)data, m_Size);
        m_Data = stbi_load_from_memory(data, m_Size, &m_Width, &m_Height, &m_Channels, 0);
    }
    
    void OpenGLTexture::LoadGPU(){
        glGenTextures(1, &m_ID); 
        glBindTexture(GL_TEXTURE_2D, m_ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // TODO: More thourough channel checking
        if(m_Channels == 4){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data);
        }
        else{
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);

    }

    void OpenGLTexture::FreeCPU(){
        stbi_image_free(m_Data);
        m_Data = nullptr;
    }
    void OpenGLTexture::FreeGPU(){
        glDeleteTextures(1, &m_ID);
        m_ID = 0;
    }


    void OpenGLTexture::Bind() const {
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }
    void OpenGLTexture::Unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
