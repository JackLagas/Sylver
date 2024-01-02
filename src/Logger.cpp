#include "Logger.hpp"

#include <iostream>
#include <fstream>
#include <format>
#include <filesystem>


bool Logger::isInitialized = false;
std::string Logger::logFile = "";



void Logger::Init(const char* logDir){
        logFile = logDir;
        logFile = logFile + "/";
        std::filesystem::path filePath{logFile + "/latest.log"};

        auto currentTimeChrono = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimeChrono);

        if(std::filesystem::exists(filePath)){

            std::time_t t = std::time(0); 
            std::tm* now = std::localtime(&t);
            std::string newName = "log";
            newName.append(std::to_string(now->tm_year + 1900));
            newName.append(std::to_string(now->tm_mon + 1));
            newName.append(std::to_string(now->tm_mday));
            newName.append(std::to_string(now->tm_hour));
            newName.append(std::to_string(now->tm_min));
            newName.append(std::to_string(now->tm_sec));
            

            std::rename((logFile + "latest.log").c_str(), (logFile + newName + ".log").c_str());

        }
        std::ofstream file;
        file.open((logFile + "latest.log").c_str(), std::ios::out);

        if(!file){
            std::cout << "Error creating file!" << std::endl;
        }
        

        file << std::ctime(&currentTime);

        file.close();

        
        isInitialized = true;

}
