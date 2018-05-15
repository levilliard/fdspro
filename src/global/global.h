#ifndef GLOBAL_H
#define GLOBAL_H

#include <istream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <sstream>

namespace GLOBAL_FUNCTION {

    std::string getDateTime(){
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
        return oss.str();
    }

    std::vector<std::string> split(const std::string &text, char sep) {
        std::vector<std::string> tokens;
        std::size_t start = 0, end = 0;
        while ((end = text.find(sep, start)) != std::string::npos) {
            if (end != start) {
              tokens.push_back(text.substr(start, end - start));
            }

            start = end + 1;
        }

        if (end != start) {
           tokens.push_back(text.substr(start));
        }

        return tokens;
    }

    std::string trim(const std::string& str)
    {
        size_t first = str.find_first_not_of(' ');
        if (std::string::npos == first)
        {
            return str;
        }
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }

}

struct DateTime{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};

#endif // GLOBAL_H
