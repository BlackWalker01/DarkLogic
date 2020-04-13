/*===--- log.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements log class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/


#include "log.h"
#include <iostream>

using namespace N_DarkLogic;

void Log::Debug(const std::string &msg)
{
    #ifndef NDEBUG
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
