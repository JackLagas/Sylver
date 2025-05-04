#include "OpenGLShader.hpp"
#include <glad/glad.h>
#include <Shaders/OpenGL/Basic_vert.h>
#include <Shaders/OpenGL/Solid_frag.h>
#include <Shaders/OpenGL/Texture_frag.h>
#include <Core/Logger.hpp>

namespace Sylver {

    OpenGLShader::OpenGLShader(eShader shader) {
        std::string vertexSource;
        std::string fragmentSource;
        vertexSource = BASIC_VERT; 
        switch(shader){
            case eShader::TEXTURE:
                fragmentSource = TEXTURE_FRAG; 
                break;
            case eShader::SOLID:
                fragmentSource = SOLID_FRAG; 
                break;
        }
        const char* vertexCode = vertexSource.c_str();
        const char* fragmentCode = fragmentSource.c_str();
        u32 vertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertShader, 1, &vertexCode, nullptr);
        glCompileShader(vertShader);
        u32 fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &fragmentCode, nullptr);
        glCompileShader(fragShader);

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertShader);
        glAttachShader(m_ID, fragShader);
        glLinkProgram(m_ID);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

    }
    OpenGLShader::~OpenGLShader() {
        glDeleteProgram(m_ID);
    }

    void OpenGLShader::Bind() const {
        glUseProgram(m_ID);
    }
    void OpenGLShader::Unbind() const {
        glUseProgram(0);
    }
    i32 OpenGLShader::GetUniformLocation(const std::string& name){
        if(m_UniformCache.find(name) == m_UniformCache.end()){
            m_UniformCache[name] = glGetUniformLocation(m_ID, name.c_str());
        }
        return m_UniformCache[name];
    }

    bool OpenGLShader::SetUniform1f(const std::string& name, f32 value) {
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform1f(location, value);
        return true;
    }
    bool OpenGLShader::SetUniform2f(const std::string& name, const glm::vec2& value) {
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform2f(location, value.x, value.y);
        return true;
    }
    bool OpenGLShader::SetUniform3f(const std::string& name, const glm::vec3& value) {

        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform3f(location, value.x, value.y, value.z);
        return true;
    }
    bool OpenGLShader::SetUniform4f(const std::string& name, const glm::vec4& value) {
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform4f(location, value.x, value.y, value.z, value.w);

        return true;
    }

    bool OpenGLShader::SetUniform1d(const std::string& name, f64 value) {

        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform1d(location, value);
        return true;
    }
    bool OpenGLShader::SetUniform2d(const std::string& name, const glm::dvec2& value) {
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform2d(location, value.x, value.y);

        return true;
    }
    bool OpenGLShader::SetUniform3d(const std::string& name, const glm::dvec3& value) {
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform3d(location, value.x, value.y, value.z);
        return true;
    }
    bool OpenGLShader::SetUniform4d(const std::string& name, const glm::dvec4& value) {
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform4d(location, value.x, value.y, value.z, value.w);

        return true;
    }

    bool OpenGLShader::SetUniform1i(const std::string& name, i32 value) {
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform1i(location, value);

        return true;
    }
    bool OpenGLShader::SetUniform2i(const std::string& name, const glm::ivec2& value) {
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform2i(location, value.x, value.y);
        return true;
    }
    bool OpenGLShader::SetUniform3i(const std::string& name, const glm::ivec3& value) {
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform3i(location, value.x, value.y, value.z);
        return true;
    }
    bool OpenGLShader::SetUniform4i(const std::string& name, const glm::ivec4& value) {
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform4i(location, value.x, value.y, value.z, value.w);
        return true;
    }

    bool OpenGLShader::SetUniform1u(const std::string& name, u32 value) {
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform1ui(location, value);
        return true;
    }
    bool OpenGLShader::SetUniform2u(const std::string& name, const glm::uvec2& value) {
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform2ui(location, value.x, value.y);
        return true;
    }
    bool OpenGLShader::SetUniform3u(const std::string& name, const glm::uvec3& value) {
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform3ui(location, value.x, value.y, value.z);
        return true;
    }
    bool OpenGLShader::SetUniform4u(const std::string& name, const glm::uvec4& value) {
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniform4ui(location, value.x, value.y, value.z, value.y);
        return true;
    }
    bool OpenGLShader::SetUniformMat2f(const std::string& name, const glm::mat2& value){
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
        return true;
    }
    bool OpenGLShader::SetUniformMat3f(const std::string& name, const glm::mat3& value){
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
        return true;
    }
    bool OpenGLShader::SetUniformMat4f(const std::string& name, const glm::mat4& value){
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
        return true;
    }

    bool OpenGLShader::SetUniformMat2d(const std::string& name, const glm::dmat2& value){
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniformMatrix2dv(location, 1, GL_FALSE, &value[0][0]);
        return true;
    }
    bool OpenGLShader::SetUniformMat3d(const std::string& name, const glm::dmat3& value){
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniformMatrix3dv(location, 1, GL_FALSE, &value[0][0]);
        return true;
    }
    bool OpenGLShader::SetUniformMat4d(const std::string& name, const glm::dmat4& value){
        i32 location = GetUniformLocation(name);
        if(location < 0){
            return false;
        }
        glUniformMatrix4dv(location, 1, GL_FALSE, &value[0][0]);
        return true;
    }

    bool OpenGLShader::SetUniform(const std::string& name, const void* data, u32 size) {
         
        Logger::Info("Uniform {}: size: {}, data ptr: {}", name, size, data);
        u32 location = glGetUniformBlockIndex(m_ID, name.c_str());
        if(location == GL_INVALID_INDEX){
            return false;
        }
        if(m_UniformBuffers.find(name) == m_UniformBuffers.end()){
            m_UniformBuffers[name] = 0;
            glGenBuffers(1, &m_UniformBuffers[name]);
            glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBuffers[name]);
            glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
        }
        glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBuffers[name]);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
        return true;
    }
}    // namespace Sylver
