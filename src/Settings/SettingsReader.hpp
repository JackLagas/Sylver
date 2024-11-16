#ifndef SYLVER_SETTINGS_READER_HPP
#define SYLVER_SETTINGS_READER_HPP


#include <Defines.hpp>
#include "Settings.hpp"

namespace Sylver {
    class SettingsReader {
        private:
            Settings m_Settings{};

        public:
            SettingsReader();

            Settings& Get() {
                return m_Settings;
            }
    };
}    // namespace Sylver

#endif
