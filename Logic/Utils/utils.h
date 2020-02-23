#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <memory>

namespace  N_Logic
{
    template<typename T>
    using ptr=std::shared_ptr<const T>;
}

/**
 * @brief isCharInVar
 * @param c
 * @return true if c can be in a name of a variable
 */
bool isCharInVar(char c);
bool isSyntaxChar(char c);
std::string getVarName(const std::string& content, size_t &i);
std::string uintToString(unsigned int x);
std::string sizeToString(const size_t& x);
#endif // UTILS_H
