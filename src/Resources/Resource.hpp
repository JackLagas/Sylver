#pragma once

#include <Defines.hpp>

namespace Sylver {
    class Resource {
        protected:
            const std::string m_Name{};
            u8* m_Data{ nullptr };

            const std::filesystem::path m_FilePath{};
            const u64 m_Offset{ 0 };
            const u64 m_Size{ 0 };

            bool m_Loaded{ false };


        public:
            Resource(const std::string& name, const std::filesystem::path& path, const bool load = false) :
                m_Name(name), m_FilePath(path), m_Offset(0), m_Size(0) {
                if (load) {
                    Load();
                }
            }
            Resource(const std::string& name, const std::filesystem::path& path, const u64 offset, const u64 size, const bool load = false) :
                m_Name(name), m_FilePath(path), m_Offset(offset), m_Size(size) {
                if (load) {
                    Load();
                }
            }
            ~Resource() {
                delete m_Data;
            }
            bool Load();
            bool Unload();
            const std::string& Name() {
                return m_Name;
            }
            const u64 Size() {
                return m_Size;
            }
            const u8* Data() {
                return m_Data;
            }
    };
}    // namespace Sylver
