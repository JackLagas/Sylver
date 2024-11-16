#ifndef SYLVER_APP_INFO_HPP
#define SYLVER_APP_INFO_HPP

#include <Defines.hpp>
#include <Common/Version.hpp>

namespace Sylver {
    class AppInfo {
        private:
            inline static std::string s_Name{ "SylverGame" };
            inline static Sylver::Version s_Version{ 0, 1, 0 };

        public:
            inline static std::string Name() {
                return s_Name;
            }
            inline static Sylver::Version Version() {
                return s_Version;
            }

            inline static void Init(const std::string& name, const Sylver::Version& version) {
                s_Name = name;
                s_Version = version;
            }
    };
}    // namespace Sylver

#endif
