#pragma once

#include <Defines.hpp>
#include <sstream>
#include <fmt/core.h>

namespace Sylver {
    struct Binary {
            u8 FormatVersion[2]{ 1, 0 };
            u32 ModuleVersion[3]{ 1, 0, 0 };
            std::vector<std::string> InheritedModulePaths{};
            struct ScriptData {
                    u64 Offset{};
                    u64 Size{};
            };
            std::unordered_map<std::string, ScriptData> Scripts{};

            struct ShaderData {
                    u64 Offset{};
                    u64 Size{};
            };
            std::unordered_map<std::string, ShaderData> Shaders{};

            struct TextureData {
                    u64 Offset{};
                    u64 Size{};
                    u8 HorizonatalTiles{ 0 };
                    u8 VerticalTiles{ 0 };
            };
            std::unordered_map<std::string, TextureData> Textures{};


            std::string ToString() {
                std::stringstream sstr;
                sstr << fmt::format("Format Version: {}.{}\n", FormatVersion[0], FormatVersion[1]);
                sstr << fmt::format("Module Version: {}.{}.{}\n", ModuleVersion[0], ModuleVersion[1], ModuleVersion[2]);
                sstr << fmt::format("Inherited Modules: {}\n", InheritedModulePaths.size());
                for (auto module : InheritedModulePaths) {
                    sstr << fmt::format("\tPath: {}\n", module);
                }
                sstr << fmt::format("Scripts: {}\n", Scripts.size());
                for (auto [name, script] : Scripts) {
                    sstr << fmt::format("\t{}: \n", name);
                    sstr << fmt::format("\t\tOffset: \n", script.Offset);
                    sstr << fmt::format("\t\tSize: \n", script.Size);
                }
                sstr << fmt::format("Shaders: {}\n", Shaders.size());
                for (auto [name, shader] : Shaders) {
                    sstr << fmt::format("\t{}: \n", name);
                    sstr << fmt::format("\t\tOffset: \n", shader.Offset);
                    sstr << fmt::format("\t\tSize: \n", shader.Size);
                }
                sstr << fmt::format("Textures: {}\n", Textures.size());
                for (auto [name, texture] : Textures) {
                    sstr << fmt::format("\t{}: \n", name);
                    sstr << fmt::format("\t\tOffset: {}\n", texture.Offset);
                    sstr << fmt::format("\t\tSize: {}\n", texture.Size);
                }

                return sstr.str();
            }
    };


}    // namespace Sylver
