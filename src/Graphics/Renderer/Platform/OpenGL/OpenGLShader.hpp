#pragma once

#include <Defines.hpp>
#include <Graphics/Renderer/Shader.hpp>


namespace Sylver{
    class OpenGLShader {
        private:
            u32 m_ID{0};
            std::unordered_map<std::string, i32> m_UniformCache;
            std::unordered_map<std::string, u32> m_UniformBuffers;
        public:
            OpenGLShader(eShader shader);
            ~OpenGLShader();

            void Bind() const;
            void Unbind() const;

            bool SetUniform1f(const std::string& name, f32 value);
            bool SetUniform2f(const std::string& name, const glm::vec2& value);
            bool SetUniform3f(const std::string& name, const glm::vec3& value);
            bool SetUniform4f(const std::string& name, const glm::vec4& value);

            bool SetUniform1d(const std::string& name, f64 value);
            bool SetUniform2d(const std::string& name, const glm::dvec2& value);
            bool SetUniform3d(const std::string& name, const glm::dvec3& value);
            bool SetUniform4d(const std::string& name, const glm::dvec4& value);

            bool SetUniform1i(const std::string& name, i32 value);
            bool SetUniform2i(const std::string& name, const glm::ivec2& value);
            bool SetUniform3i(const std::string& name, const glm::ivec3& value);
            bool SetUniform4i(const std::string& name, const glm::ivec4& value);

            bool SetUniform1u(const std::string& name, u32 value);
            bool SetUniform2u(const std::string& name, const glm::uvec2& value);
            bool SetUniform3u(const std::string& name, const glm::uvec3& value);
            bool SetUniform4u(const std::string& name, const glm::uvec4& value);

            bool SetUniformMat2f(const std::string& name, const glm::mat2& value);
            bool SetUniformMat3f(const std::string& name, const glm::mat3& value);
            bool SetUniformMat4f(const std::string& name, const glm::mat4& value);
            
            bool SetUniformMat2d(const std::string& name, const glm::dmat2& value);
            bool SetUniformMat3d(const std::string& name, const glm::dmat3& value);
            bool SetUniformMat4d(const std::string& name, const glm::dmat4& value);

            bool SetUniform(const std::string& name, const void* data, u32 size);

        private:
            i32 GetUniformLocation(const std::string& name);
    };
}
