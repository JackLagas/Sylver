#ifndef SYLVER_RESOURCES_MANAGER_HPP
#define SYLVER_RESOURCES_MANAGER_HPP


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

            static Result Add(const std::string& name, const ResourceType type, const std::filesystem::path& path, const b8 load = false);
            static Result Add(const std::string& name, const ResourceType type, const std::filesystem::path& path, const u64 offset, const u64 size, const b8 load = false);
            static b8 Remove(const std::string& name);

            static b8 Load(const std::string& name);
            static b8 Unload(const std::string& name);

            static Resource* Get(const std::string& name);

            static void Cleanup();
    };
}    // namespace Sylver

#endif
