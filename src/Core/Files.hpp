#ifndef SYLVER_FILES_HPP
#define SYLVER_FILES_HPP

#include <Defines.hpp>

#include <Core/Logger.hpp>

#include <filesystem>


#if defined(SYLVER_PLATFORM_WINDOWS)
#include <window.h>
#elif defined(SYLVER_PLATFORM_LINUX)
#include <pwd.h>
#endif

namespace Sylver {
    // TODO: Windows and MacOS Testing
    class Files {
        private:
            static inline std::filesystem::path s_ExecutablePath{};
            static inline std::filesystem::path s_ApplicationPath{};

            static inline std::filesystem::path s_UserPath{};

            static inline std::filesystem::path s_WorkingPath{};

        public:
            /**
             * @brief Initializes path variables to accessing files regardless of working directory.
             *
             * @param arg0 the first command-line argument which gives the
             * relative path of the executable.
             *
             */
            static inline void Init(const std::string& arg0, const std::string& gamePath = "game") {
                s_ExecutablePath = std::filesystem::weakly_canonical(arg0).parent_path();
                s_ApplicationPath = s_ExecutablePath / gamePath;
                s_WorkingPath = s_ExecutablePath;
                s_UserPath = s_ExecutablePath;
            }

            static inline void InitUserPath(const std::string& appName) {
                std::string path;
                // TODO: Test on Windows
#if defined(SYLVER_PLATFORM_WINDOWS)
                PWSTR ppszPath;    // variable to receive the path memory block pointer.
                HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &ppszPath);

                if (SUCCEEDED(hr)) {
                    path = ppszPath;    // make a local copy of the path
                }

                CoTaskMemFree(ppszPath);    // free up the path memory block
                std::filesystem::path documentsFolder = std::filesystem::weakly_canonical(path);
                if (!std::filesystem::exists(documentsFolder / "My Games")) {
                    if (!std::filesystem::create_directory(documentsFolder / "My Games")) {
                        return;
                    }
                }
                if (!std::filesystem::exists(documentsFolder / "My Games" / appName)) {
                    if (!std::filesystem::create_directory(documentsFolder / "My Games" / appName)) {
                        return;
                    }
                }
                s_UserPath = documentsFolder / "My Games" / appName;
#elif defined(SYLVER_PLATFORM_LINUX)
                const char* homeDirptr;
                if ((homeDirptr = getenv("HOME")) == nullptr) {
                    homeDirptr = getpwuid(getuid())->pw_dir;
                }
                std::filesystem::path homeDir = std::filesystem::weakly_canonical(homeDirptr);
                if (!std::filesystem::exists(homeDir / ".config")) {
                    if (!std::filesystem::create_directory(homeDir / ".config")) {
                        return;
                    }
                }
                if (!std::filesystem::exists(homeDir / ".config" / appName)) {
                    if (!std::filesystem::create_directory(homeDir / ".config" / appName)) {
                        return;
                    }
                }
                s_UserPath = homeDir / ".config" / appName;

#elif defined(SYLVER_PLATFORM_MAC)
                // TODO: Mac user folder
#endif
            }

            static inline std::filesystem::path ExecutablePath() {
                return s_ExecutablePath;
            }

            static inline std::filesystem::path RelativeExecutable(const std::string& path) {
                return s_ExecutablePath / path;
            }
            static inline std::filesystem::path ApplicationPath() {
                return s_ApplicationPath;
            }

            static inline std::filesystem::path RelativeApplication(const std::string& path) {
                return s_ApplicationPath / path;
            }
            static inline std::filesystem::path UserPath() {
                return s_UserPath;
            }

            static inline std::filesystem::path RelativeUser(const std::string& path) {
                return s_UserPath / path;
            }
    };

}    // namespace Sylver


#endif
