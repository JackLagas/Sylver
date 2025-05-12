#pragma once

#include <Defines.hpp>
#include <IO/Logger.hpp>
#include <filesystem>

namespace Sylver {
    class Files {
        private:
            inline static Files* s_Instance{nullptr};

            std::filesystem::path m_ExecutablePath{};

            std::filesystem::path m_UserPath{};

            std::filesystem::path m_WorkingPath{};

            Files(const std::string& arg0) {
                m_ExecutablePath = std::filesystem::weakly_canonical(arg0).parent_path();
                m_WorkingPath = std::filesystem::current_path();
                //TODO: Get User path
                m_UserPath = m_ExecutablePath;
            }

        public:
            /**
             * @brief Initializes path variables to accessing files regardless of working directory.
             *
             * @param arg0 the first command-line argument which gives the
             * relative path of the executable.
             *
             */
            static inline void Init(const std::string& arg0) {
                s_Instance = new Files(arg0);
            }

            static inline const std::filesystem::path& ExecutablePath() {
                return s_Instance->m_ExecutablePath;
            }
            static inline const std::filesystem::path RelativeExecutable(const std::string& path) {
                return s_Instance->m_ExecutablePath / path;
            }

            static inline const std::filesystem::path& WorkingPath() {
                return s_Instance->m_WorkingPath;
            }
            static inline const std::filesystem::path RelativeWorking(const std::string& path) {
                return s_Instance->m_WorkingPath / path;
            }

            static inline const std::filesystem::path& UserPath() {
                return s_Instance->m_UserPath;
            }
            static inline const std::filesystem::path RelativeUser(const std::string& path) {
                return s_Instance->m_UserPath / path;
            }
    };

}    // namespace Sylver
