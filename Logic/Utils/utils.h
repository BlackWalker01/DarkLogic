#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <memory>
#include <complex>

namespace  N_Logic
{
    //Main pointer for Logic
    template<typename T>
    using ptr=std::shared_ptr<const T>;

    /**
      * Define basic types for Logic
      */
    using UInt = size_t; //Natural Integer
    using Int = long long int; //Integer
    //using Rational = rational; //Rational
    using Real = double; //Real
    using Complex = std::complex<Real>;
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
