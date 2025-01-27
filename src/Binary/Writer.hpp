#ifndef SYLVER_BINARY_WRITER_HPP
#define SYLVER_BINARY_WRITER_HPP

#include <Defines.hpp>

#include <Common/Result.hpp>

#include <fstream>

namespace Sylver {
    class BinaryWriter {
        private:
            u8 m_FormatVersion[2]{ 1, 0 };
            u32 m_ModuleVersion[3]{ 1, 0, 0 };
            u8 m_InheritedModuleCount{ 0 };
            std::vector<std::string> m_InheritedModulePaths{};

            u32 m_ScriptCount{ 0 };
            struct ScriptData {
                    std::string Name{};
                    std::filesystem::path Path{};
            };
            std::vector<ScriptData> m_Scripts{};

            u32 m_ShaderCount{ 0 };
            struct ShaderData {
                    std::string Name{};
                    std::filesystem::path Path{};
            };
            std::vector<ShaderData> m_Shaders{};

            u32 m_TextureCount{ 0 };
            struct TextureData {
                    std::string Name{};
                    std::filesystem::path Path{};
                    u8 HorizonatalTiles{ 0 };
                    u8 VerticalTiles{ 0 };
            };
            std::vector<TextureData> m_Textures{};

        public:
            BinaryWriter() {
            }

            ~BinaryWriter() {
            }

            void SetFormatVersion(const u8 version[2]) {
                m_FormatVersion[0] = version[0];
                m_FormatVersion[1] = version[1];
            }
            void SetFormatVersion(const u8 versionMajor, const u8 versionMinor) {
                m_FormatVersion[0] = versionMajor;
                m_FormatVersion[1] = versionMinor;
            }
            void SetModuleVersion(const u32 version[3]) {
                m_ModuleVersion[0] = version[0];
                m_ModuleVersion[1] = version[1];
                m_ModuleVersion[2] = version[2];
            }
            void SetModuleVersion(const u32 versionMajor, const u32 versionMinor, const u32 versionPatch) {
                m_ModuleVersion[0] = versionMajor;
                m_ModuleVersion[1] = versionMinor;
                m_ModuleVersion[2] = versionPatch;
            }
            void AddInheritedModule(const std::string& path) {
                m_InheritedModuleCount++;
                m_InheritedModulePaths.push_back(path);
            }
            void AddScript(const std::string& name, const std::filesystem::path& path) {
                m_ScriptCount++;
                m_Scripts.push_back(ScriptData(name, path));
            }
            void AddShader(const std::string& name, const std::filesystem::path& path) {
                m_ShaderCount++;
                m_Shaders.push_back(ShaderData(name, path));
            }
            void AddTexture(const std::string& name, const std::filesystem::path& path) {
                m_TextureCount++;
                m_Textures.push_back(TextureData(name, path, 0, 0));
            }
            void AddTexture(const std::string& name, const std::filesystem::path& path, const u8 horizontalTiles, const u8 verticalTiles) {
                m_TextureCount++;
                m_Textures.push_back(TextureData(name, path, horizontalTiles, verticalTiles));
            }

            Result Write(const std::filesystem::path& outputFile) {
                if (std::filesystem::exists(outputFile)) {
                    return Result::FILE_EXISTS;
                }
                if (!std::filesystem::exists(outputFile.parent_path())) {
                    return Result::NO_SUCH_DIRECTORY;
                }
                if (m_FormatVersion[0] == 1 && m_FormatVersion[1] == 0) {
                    return WriteV1_0(outputFile);
                } else {
                    return Result::INVALID_INPUT;
                }
            }


        private:
            Result AppendFile(std::ofstream& outputStream, const std::filesystem::path& inputPath){
                std::ifstream inputStream(inputPath, std::ios::in | std::ios::binary);

                std::ostringstream sstr;
                sstr << inputStream.rdbuf();
                std::string str = sstr.str();
                outputStream.write(str.c_str(), str.length());

                return Result::OK;
            }
            Result WriteV1_0(const std::filesystem::path& outputFile) {
                std::ofstream fileStream(outputFile, std::ios::out | std::ios::binary | std::ios::app);

                fileStream.write("SLVB", 4);
                fileStream.write((char*)m_FormatVersion, sizeof(m_FormatVersion));
                fileStream.write((char*)m_ModuleVersion, sizeof(m_ModuleVersion));
                fileStream.write((char*)&m_InheritedModuleCount, sizeof(m_InheritedModuleCount));
                for (auto& modulePath : m_InheritedModulePaths) {
                    u16 length = static_cast<u16>(modulePath.length());
                    fileStream.write((char*)&length, sizeof(length));
                    fileStream.write(modulePath.c_str(), length);
                }
                u64 scriptDataSize = 4 + 8 * m_ScriptCount;
                for (auto& script : m_Scripts) {
                    scriptDataSize += script.Name.length() + 1;
                    scriptDataSize += 8;
                    scriptDataSize += std::filesystem::file_size(script.Path);
                }
                u64 shaderDataSize = 4 + 8 * m_ShaderCount;
                for (auto& shader : m_Shaders) {
                    shaderDataSize += shader.Name.length() + 1;
                    shaderDataSize += 8;
                    shaderDataSize += std::filesystem::file_size(shader.Path);
                }
                u64 textureDataSize = 4 + 8 * m_TextureCount;
                for (auto& texture : m_Textures) {
                    textureDataSize += texture.Name.length() + 1;
                    textureDataSize += 8 + 2;
                    textureDataSize += std::filesystem::file_size(texture.Path);
                }
                u64 scriptLocation = (u64)fileStream.tellp() + 48;
                u64 shaderLocation = scriptLocation + scriptDataSize;
                u64 textureLocation = shaderLocation + shaderDataSize;

                fileStream.write((char*)&scriptLocation, sizeof(scriptLocation));
                fileStream.write((char*)&shaderLocation, sizeof(shaderLocation));
                fileStream.write((char*)&textureLocation, sizeof(textureLocation));
                u8 zero = 0;
                for(u32 i = 0; i < 24; i++){
                    fileStream.write((char*)&zero, 1);
                }

                // Assets

                // Scripts

                fileStream.write((char*)&m_ScriptCount, sizeof(m_ScriptCount));

                u64 scriptOffset = scriptLocation + 4 + 8 * m_ScriptCount;
                for (auto& script : m_Scripts) {
                    fileStream.write((char*)&scriptOffset, sizeof(scriptOffset));
                    scriptOffset += std::filesystem::file_size(script.Path) + 1 + script.Name.length() + 8;
                }
                for (auto& script : m_Scripts) {
                    u8 nameSize = script.Name.length();
                    fileStream.write((char*)&nameSize, sizeof(nameSize));
                    fileStream.write((char*)script.Name.c_str(), nameSize);
                    u64 fileSize = std::filesystem::file_size(script.Path);
                    fileStream.write((char*)&fileSize, sizeof(fileSize));
                    AppendFile(fileStream, script.Path);
                }
                // Shaders

                fileStream.write((char*)&m_ShaderCount, sizeof(m_ShaderCount));

                u64 shaderOffset = shaderLocation + 4 + 8 * m_ShaderCount;
                for (auto& shader : m_Shaders) {
                    fileStream.write((char*)&shaderOffset, sizeof(shaderOffset));
                    shaderOffset += std::filesystem::file_size(shader.Path) + 1 + shader.Name.length() + 8;
                }
                for (auto& shader : m_Shaders) {
                    u8 nameSize = shader.Name.length();
                    fileStream.write((char*)&nameSize, sizeof(nameSize));
                    fileStream.write((char*)shader.Name.c_str(), nameSize);
                    u64 fileSize = std::filesystem::file_size(shader.Path);
                    fileStream.write((char*)&fileSize, sizeof(fileSize));
                    AppendFile(fileStream, shader.Path);
                }
                // Textures

                fileStream.write((char*)&m_TextureCount, sizeof(m_TextureCount));

                u64 textureOffset = textureLocation + 4 + 8 * m_TextureCount;
                for (auto& texture : m_Textures) {
                    fileStream.write((char*)&textureOffset, sizeof(textureOffset));
                    textureOffset += std::filesystem::file_size(texture.Path) +    // File size
                                     1 + texture.Name.length() +                   // Name
                                     2 + 8;                                        // Tiling data + file size data
                }
                for (auto& texture : m_Textures) {
                    u8 nameSize = texture.Name.length();
                    fileStream.write((char*)&nameSize, sizeof(nameSize));
                    fileStream.write((char*)texture.Name.c_str(), nameSize);
                    fileStream.write((char*)&texture.HorizonatalTiles, sizeof(texture.HorizonatalTiles));
                    fileStream.write((char*)&texture.VerticalTiles, sizeof(texture.VerticalTiles));
                    u64 fileSize = std::filesystem::file_size(texture.Path);
                    fileStream.write((char*)&fileSize, sizeof(fileSize));

                    AppendFile(fileStream, texture.Path);
                }


                fileStream.close();
                return Result::OK;
            }
    };
}    // namespace Sylver


#endif
