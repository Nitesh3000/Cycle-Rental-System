#include "utils/Logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

void Logger::log(const std::string& level, const std::string& message) {
    std::time_t now = std::time(nullptr);
    std::tm localTime;
#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif
    std::stringstream ss;
    ss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    std::cout << "[" << ss.str() << "] " << level << ": " << message << "\n";
}
