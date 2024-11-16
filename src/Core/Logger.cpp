#include "Logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace Sylver {
    void Logger::Init(std::string logPath) {
        std::vector<spdlog::sink_ptr> sinks = {
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
            std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                "Engine.log",
                10 * 1024 * 1024,
                10)
        };
        s_Logger = std::make_shared<spdlog::logger>("SylverLogger", begin(sinks), end(sinks));
    }
}    // namespace Sylver
