#include "log.h"
#include <iostream>

using namespace N_Logic;

void Log::Debug(const std::string &msg)
{
    #ifdef NDEBUG
    std::cout<<"[DEBUG] "<<msg<<std::endl;
    #endif
}

void Log::Info(const std::string &msg)
{
    std::cout<<"[INFO] "<<msg<<std::endl;
}

void Log::Error(const std::string &msg)
{
    std::cout<<"[ERROR] "<<msg<<std::endl;
}
