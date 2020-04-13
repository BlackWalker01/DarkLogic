/*===--- log.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines Log in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_LOG_H
#define DARK_LOGIC_LOG_H
#include <string>

namespace N_DarkLogic
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
#endif // DARK_LOGIC_LOG_H
