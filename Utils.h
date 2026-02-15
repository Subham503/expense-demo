#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <algorithm>

namespace Utils {
    // Get current date as string YYYY-MM-DD
    inline std::string getCurrentDate() {
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        std::stringstream ss;
        ss << (now->tm_year + 1900) << "-" 
           << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << "-" 
           << std::setw(2) << std::setfill('0') << now->tm_mday;
        return ss.str();
    }

    // Trim whitespace from string
    inline std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (std::string::npos == first) {
            return str;
        }
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }

    // Simple clear screen
    inline void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
}

#endif // UTILS_H
