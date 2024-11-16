#include "Module.hpp"

#include <Core/Files.hpp>

#include <Core/Logger.hpp>

#include <nlohmann/json.hpp>

#include <fstream>

namespace Sylver {


    enum class DescriptorExtension {
        JSON,
        TOML
    };

    std::string ReadFile(std::filesystem::path path) {
        std::ifstream file;
        std::stringstream filestream;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            file.open(path);
            filestream << file.rdbuf();
            file.close();
        } catch (std::ifstream::failure e) {
            Logger::Warn("Failed to read file: {}, with error {}", path.string(), e.what());
        }
        return filestream.str();
    }

    // TODO: manage inherited assets
    std::optional<Module> ParseJson(std::filesystem::path path, std::string moduleName) {
        std::string jsonString = ReadFile(path);
        std::ifstream file(path);
        nlohmann::json data = nlohmann::json::parse(file);
        Module module;
        if (data.contains("name")) {
            std::string name = data["name"];
            if (name != moduleName) {
                Logger::Warn("Module name mismatch {} != {}", moduleName, name);
            }
        } else {
            Logger::Warn("Module name not specified. Using: {}", moduleName);
        }
        module.name = moduleName;
        if (data.contains("version")) {
            std::optional<Version> version = Version::FromString(data["version"]);
            if (!version.has_value()) {
                Logger::Warn("Invalid version string {} Defaulting to 1.0.0", (std::string)data["version"]);
                module.version = Version(1, 0, 0);
            } else {
                module.version = version.value();
            }
        } else {
            module.version = Version(1, 0, 0);
        }
        if (data.contains("assets")) {
            if (data["assets"].contains("textures")) {
                nlohmann::json textures = data["assets"]["textures"];
                for (auto& [name, relPath] : textures.items()) {
                    module.textures[name] = path.parent_path() / "textures" / relPath;
                }
            }
            if (data["assets"].contains("scripts")) {
                nlohmann::json scripts = data["assets"]["scripts"];
                for (auto& [name, relPath] : scripts.items()) {
                    module.scripts[name] = path.parent_path() / "scripts" / relPath;
                }
            }
            if (data["assets"].contains("shaders")) {
                nlohmann::json shaders = data["assets"]["shaders"];
                for (auto& [name, relPath] : shaders.items()) {
                    module.shaders[name] = path.parent_path() / "shaders" / relPath;
                }
            }
        }
        return module;
    }

    std::optional<Module>
    Module::Load(std::string path) {
        std::filesystem::path modulePath = Files::RelativeApplication(path);
        if (!std::filesystem::is_directory(modulePath)) {
            Logger::Warn("Module is not a directory! {}", modulePath.string());
            return std::optional<Module>();
        }
        std::filesystem::path moduleDescriptor;
        DescriptorExtension extensionType;

        if (std::filesystem::exists(modulePath / modulePath.filename().string().append(".json"))) {
            moduleDescriptor = modulePath / modulePath.filename().string().append(".json");
            extensionType = DescriptorExtension::JSON;
        } else if (std::filesystem::exists(modulePath / "index.json")) {
            moduleDescriptor = modulePath / "index.json";
            extensionType = DescriptorExtension::JSON;
        } else if (std::filesystem::exists(modulePath / "meta.json")) {
            moduleDescriptor = modulePath / "meta.json";
            extensionType = DescriptorExtension::JSON;
        } else if (std::filesystem::exists(modulePath / "descriptor.json")) {
            moduleDescriptor = modulePath / "descriptor.json";
            extensionType = DescriptorExtension::JSON;
        } else if (std::filesystem::exists(modulePath / modulePath.filename().string().append(".toml"))) {
            moduleDescriptor = modulePath / modulePath.filename().string().append(".toml");
            extensionType = DescriptorExtension::TOML;
        } else if (std::filesystem::exists(modulePath / "index.toml")) {
            moduleDescriptor = modulePath / "index.toml";
            extensionType = DescriptorExtension::TOML;
        } else if (std::filesystem::exists(modulePath / "meta.toml")) {
            moduleDescriptor = modulePath / "meta.toml";
            extensionType = DescriptorExtension::TOML;
        } else if (std::filesystem::exists(modulePath / "descriptor.toml")) {
            moduleDescriptor = modulePath / "descriptor.toml";
            extensionType = DescriptorExtension::TOML;
        } else {
            Logger::Warn("Could not find Module descriptor for {}", modulePath.filename().string());
        }

        if (extensionType == DescriptorExtension::JSON) {
            return ParseJson(moduleDescriptor, modulePath.filename().string());
        }


        return Module();
    }
}    // namespace Sylver
