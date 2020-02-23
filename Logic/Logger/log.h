#ifndef LOG_H
#define LOG_H
#include <string>

namespace N_Logic
{
class Log
{
public:
    Log() = default;

    static void Debug(const std::string& msg);
    static void Info(const std::string& msg);
    static void Error(const std::string& msg);

    ~Log() = default;
};
}
#endif // LOG_H
