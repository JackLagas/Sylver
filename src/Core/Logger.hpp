#pragma once

#include <Defines.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/pattern_formatter.h>


#if defined(SYLVER_LOG_VULKAN_CREATIONS)
#define VULKAN_CREATED(item)   Logger::Info("Created {}", item)
#define VULKAN_ALLOCATED(item) Logger::Info("Allocated {}", item)
#else
#define VULKAN_CREATED(item)
#define VULKAN_ALLOCATED(item)
#endif

namespace Sylver {
    class Logger {
        private:
            inline static std::shared_ptr<spdlog::logger> s_Logger;

        public:
            static void Init(std::string logPath);

            inline static std::shared_ptr<spdlog::logger> Get() {
                return s_Logger;
            }
            template<typename T>
            inline static void Log(spdlog::level::level_enum level, const T& msg) {
                s_Logger->log(level, msg);
            }
            template<typename T>
            inline static void Trace(const T& msg) {
                s_Logger->log(spdlog::level::trace, msg);
            }
            template<typename... Args>
            inline static void Trace(spdlog::format_string_t<Args...> fmt, Args&&... args) {
                s_Logger->log(spdlog::level::trace, fmt, std::forward<Args>(args)...);
            }
            template<typename T>
            inline static void Debug(const T& msg) {
                s_Logger->log(spdlog::level::debug, msg);
            }
            template<typename... Args>
            inline static void Debug(spdlog::format_string_t<Args...> fmt, Args&&... args) {
                s_Logger->log(spdlog::level::debug, fmt, std::forward<Args>(args)...);
            }

            template<typename T>
            inline static void Info(const T& msg) {
                s_Logger->log(spdlog::level::info, msg);
            }
            template<typename... Args>
            inline static void Info(spdlog::format_string_t<Args...> fmt, Args&&... args) {
                s_Logger->log(spdlog::level::info, fmt, std::forward<Args>(args)...);
            }
            template<typename T>
            inline static void Warn(const T& msg) {
                s_Logger->log(spdlog::level::warn, msg);
            }
            template<typename... Args>
            inline static void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args) {
                s_Logger->log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
            }
            template<typename T>
            inline static void Error(const T& msg) {
                s_Logger->log(spdlog::level::err, msg);
            }
            template<typename... Args>
            inline static void Error(spdlog::format_string_t<Args...> fmt, Args&&... args) {
                s_Logger->log(spdlog::level::err, fmt, std::forward<Args>(args)...);
            }
            template<typename T>
            inline static void Critical(const T& msg) {
                s_Logger->log(spdlog::level::critical, msg);
            }
            template<typename... Args>
            inline static void Critical(spdlog::format_string_t<Args...> fmt, Args&&... args) {
                s_Logger->log(spdlog::level::critical, fmt, std::forward<Args>(args)...);
            }
    };
}    // namespace Sylver

