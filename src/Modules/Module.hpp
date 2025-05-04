#pragma once

#include <Defines.hpp>
#include <Common/Version.hpp>
#include <fmt/core.h>


namespace Sylver {

    struct Module {
            std::string name;
            Version version;
            std::map<std::string, std::filesystem::path> textures;
            std::map<std::string, std::filesystem::path> shaders;
            std::map<std::string, std::filesystem::path> scripts;


            static std::optional<Module> Load(std::string path);
    };

}    // namespace Sylver
