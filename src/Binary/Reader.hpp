#pragma once
#include "Binary.hpp"

#include <fstream>

#include <Common/Result.hpp>

namespace Sylver {
    class BinaryReader {
        private:
            Binary m_Binary{ 0 };

        public:
            BinaryReader() {
            }
            BinaryReader(std::string path) {
            }

            ~BinaryReader() {
            }


            Result Read(std::filesystem::path path) {
                if (!std::filesystem::exists(path)) {
                    return Result::FILE_NOT_FOUND;
                }
                std::ifstream fileReader(path, std::ios::in | std::ios::binary);
                u8 header[4]{};
                fileReader.seekg(0);
                fileReader.read((char*)&header, 4);
                if (!(header[0] == 'S' && header[1] == 'L' && header[2] == 'V' && header[3] == 'B')) {
                    return Result::FILE_CORRUPTED;
                }
                fileReader.seekg(4);
                fileReader.read((char*)&m_Binary.FormatVersion, 2);
                if (m_Binary.FormatVersion[0] == 1) {
                    if (m_Binary.FormatVersion[1] == 0) {
                        return ReadV1_0(fileReader);
                    }
                }
                return Result::FILE_CORRUPTED;
            }

            Binary Get() {
                return m_Binary;
            }

        private:
            Result ReadV1_0(std::ifstream& fileReader) {
                fileReader.seekg(6);
                fileReader.read((char*)&m_Binary.ModuleVersion, sizeof(m_Binary.ModuleVersion));

                // Inherited Modules
                u8 inheritedModuleCount;
                fileReader.read((char*)&inheritedModuleCount, sizeof(inheritedModuleCount));
                m_Binary.InheritedModulePaths.resize(inheritedModuleCount);
                u64* inheritedModuleOffsets = new u64[inheritedModuleCount];
                fileReader.read((char*)inheritedModuleOffsets, inheritedModuleCount * sizeof(u64));
                for (std::string& path : m_Binary.InheritedModulePaths) {
                    u16 pathLength;
                    fileReader.read((char*)&pathLength, sizeof(pathLength));
                    path.resize(pathLength);
                    fileReader.read(path.data(), pathLength);
                }


                // Index
                u64 scriptOffset;
                u64 shaderOffset;
                u64 textureOffset;
                u64 dummy;
                fileReader.read((char*)&scriptOffset, sizeof(scriptOffset));
                fileReader.read((char*)&shaderOffset, sizeof(shaderOffset));
                fileReader.read((char*)&textureOffset, sizeof(textureOffset));
                fileReader.read((char*)&dummy, sizeof(dummy));
                fileReader.read((char*)&dummy, sizeof(dummy));
                fileReader.read((char*)&dummy, sizeof(dummy));

                // Scripts

                fileReader.seekg(scriptOffset);
                {
                    u32 scriptCount;
                    fileReader.read((char*)&scriptCount, sizeof(scriptCount));
                    std::vector<u64> scriptOffsets(scriptCount);
                    fileReader.read((char*)scriptOffsets.data(), scriptCount * sizeof(u64));
                    for (u32 i = 0; i < scriptCount; i++) {
                        u8 nameLength;
                        std::string name;
                        u64 size;
                        fileReader.seekg(scriptOffsets[i]);
                        fileReader.read((char*)&nameLength, sizeof(nameLength));
                        name.resize(nameLength);
                        fileReader.read(name.data(), nameLength);
                        fileReader.read((char*)&size, sizeof(size));
                        if (m_Binary.Scripts.find(name) != m_Binary.Scripts.end()) {
                            return Result::FILE_CORRUPTED;
                        }
                        m_Binary.Scripts[name] = { .Offset = scriptOffsets[i], .Size = size };
                    }
                }
                // Shaders

                fileReader.seekg(shaderOffset);
                {
                    u32 shaderCount;
                    fileReader.read((char*)&shaderCount, sizeof(shaderCount));
                    std::vector<u64> shaderOffsets(shaderCount);
                    fileReader.read((char*)shaderOffsets.data(), shaderCount * sizeof(u64));
                    for (u32 i = 0; i < shaderCount; i++) {
                        u8 nameLength;
                        std::string name;
                        u64 size;
                        fileReader.seekg(shaderOffsets[i]);
                        fileReader.read((char*)&nameLength, sizeof(nameLength));
                        name.resize(nameLength);
                        fileReader.read(name.data(), nameLength);
                        fileReader.read((char*)&size, sizeof(size));
                        if (m_Binary.Shaders.find(name) != m_Binary.Shaders.end()) {
                            return Result::FILE_CORRUPTED;
                        }
                        m_Binary.Shaders[name] = { .Offset = shaderOffsets[i], .Size = size };
                    }
                }
                // Textures

                fileReader.seekg(textureOffset);
                {
                    u32 textureCount;
                    fileReader.read((char*)&textureCount, sizeof(textureCount));
                    std::vector<u64> textureOffsets(textureCount);
                    fileReader.read((char*)textureOffsets.data(), textureCount * sizeof(u64));
                    for (u32 i = 0; i < textureCount; i++) {
                        u8 nameLength;
                        std::string name;
                        u64 size;
                        u8 hTiles;
                        u8 vTiles;
                        fileReader.seekg(textureOffsets[i]);
                        fileReader.read((char*)&nameLength, sizeof(nameLength));
                        name.resize(nameLength);
                        fileReader.read(name.data(), nameLength);
                        fileReader.read((char*)&hTiles, sizeof(hTiles));
                        fileReader.read((char*)&vTiles, sizeof(vTiles));
                        fileReader.read((char*)&size, sizeof(size));
                        if (m_Binary.Textures.find(name) != m_Binary.Textures.end()) {
                            return Result::FILE_CORRUPTED;
                        }
                        m_Binary.Textures[name] = {
                            .Offset = textureOffsets[i],
                            .Size = size,
                            .HorizonatalTiles = hTiles,
                            .VerticalTiles = vTiles
                        };
                    }
                }


                return Result::OK;
            }
    };
}    // namespace Sylver
