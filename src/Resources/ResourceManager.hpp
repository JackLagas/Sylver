#pragma once

#include <Defines.hpp>
#include "Resource.hpp"
#include <Common/Result.hpp>

namespace Sylver {
    class ResourceManager {
        private:
            inline static std::unordered_map<std::string, Resource*> s_Resources{};

        public:
            ResourceManager() = delete;
            ResourceManager(const ResourceManager&) = delete;

            static Result Add(const std::string& name, const std::filesystem::path& path, const bool load = false);
            static Result Add(const std::string& name, const std::filesystem::path& path, const u64 offset, const u64 size, const bool load = false);
            static bool Remove(const std::string& name);

            static bool Load(const std::string& name);
            static bool Unload(const std::string& name);

            static Resource* Get(const std::string& name);

            static void Cleanup();
    };
}    // namespace Sylver
