#ifndef SYLVER_MODULE_HPP
#define SYLVER_MODULE_HPP

#include <Defines.hpp>
#include <Common/Version.hpp>


namespace Sylver {

    struct Module{
        std::string name;
        Version version;
        std::map<std::string, std::filesystem::path> textures;
        std::map<std::string, std::filesystem::path> shaders;
        std::map<std::string, std::filesystem::path> scripts;
        

        static std::optional<Module> Load(std::string path);
    };

}

#endif
