#pragma once

#include <Defines.hpp>
#include <Common/Version.hpp>

namespace Sylver {
    // TODO: Fill in information during build process
    class EngineInfo {
        private:
            inline static std::string s_Name{ "SylverEngine" };
            inline static Sylver::Version s_Version{ 0, 1, 0 };

        public:
            inline static std::string Name() {
                return s_Name;
            }
            inline static Sylver::Version Version() {
                return s_Version;
            }
    };
}    // namespace Sylver
