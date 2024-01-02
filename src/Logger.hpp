#ifndef LOG_LEVEL
#define LOG_LEVEL 4
#endif

#ifndef LOGGER_H
#define LOGGER_H



#if LOG_LEVEL < 6
#define INFO(message, ...)
#else
#define INFO(message, ...) Logger::Log(LOG_LEVELS::INFO, message, __VA_ARGS__)
#endif

#if LOG_LEVEL < 5
#define TRACE(message, ...)
#else
#define TRACE(message, ...) Logger::Log(LOG_LEVELS::TRACE, message, __VA_ARGS__)
#endif

#if LOG_LEVEL < 4
#define DEBUG(message, ...)
#else
#define DEBUG(message, ...) Logger::Log(LOG_LEVELS::DEBUG, message, __VA_ARGS__)
#endif

#if LOG_LEVEL < 3
#define WARN(message, ...)
#else
#define WARN(message, ...) Logger::Log(LOG_LEVELS::WARN, message, __VA_ARGS__)
#endif

#if LOG_LEVEL < 2
#define ERROR(message, ...)
#else
#define ERROR(message, ...) Logger::Log(LOG_LEVELS::ERROR, message, __VA_ARGS__)
#endif

#if LOG_LEVEL < 1
#define FATAL(message, ...)
#else
#define FATAL(message, ...) Logger::Log(LOG_LEVELS::FATAL, message, __VA_ARGS__)
#endif

#include <string>
#include <cstdarg>
#include <string_view>


#include <iostream>
#include <fstream>
#include <format>
#include <filesystem>



enum LOG_LEVELS{
    INFO = 0,
    TRACE = 1,
    DEBUG = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5,
};

const std::string logLevelString[6] = {
    "Info",
    "Trace",
    "Debug",
    "Warning",
    "Error",
    "Fatal"
};

class Logger{

    private:
        static std::string logFile;
        static bool isInitialized;
        
    public:
        template <typename... Args>
        static void Log(LOG_LEVELS level, const std::string_view message, Args&&... args);

        static void Init(const char* logDir);
        
};


template <typename... Args>
void Logger::Log(LOG_LEVELS level, const std::string_view message, Args&&... args){

    if(!isInitialized){
        std::cout << "[FATAL]: Logger was not initialized" << std::endl;
        return;

    }

    std::string finalMessage = std::format("[{}]: ", logLevelString[level]);
    
    finalMessage += std::vformat(message, std::make_format_args(args...));

    std::cout << finalMessage << std::endl;

    std::ofstream file;
    file.open((logFile + "latest.log").c_str(), std::ios_base::app);
    file << finalMessage << "\n";
    file.close();

}

#endif
