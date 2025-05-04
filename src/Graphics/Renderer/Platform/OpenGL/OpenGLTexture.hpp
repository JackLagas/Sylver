#pragma once

#include <Graphics/Renderer/Texture.hpp>

#include <Defines.hpp>



namespace Sylver{
    class OpenGLTexture : public Texture{
        private:
            const std::filesystem::path m_FilePath;
            const u32 m_Offset{0};
            const u32 m_Size{0};
            u32 m_ID{0};
            u8* m_Data{nullptr};
            i32 m_Width{0};
            i32 m_Height{0};
            i32 m_Channels{0};
        public:
            OpenGLTexture(const std::filesystem::path& path);
            OpenGLTexture(const std::filesystem::path& path, const u32 offset, const u32 size);
            ~OpenGLTexture();

            void LoadCPU() override;
            void LoadGPU() override;

            void FreeCPU() override;
            void FreeGPU() override;

            bool InCPU() const override;
            bool InGPU() const override;

            void Bind() const override;
            void Unbind() const override;


    };
}
