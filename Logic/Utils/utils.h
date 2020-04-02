#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <memory>
#include <complex>
#include <concepts>

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

    //pre-define Set Types
    class BooleanSet;
    class NaturalIntegerSet;

    //pre-define Variable Types
    class Boolean;
    class ConstBoolean;

    //define concepts
    template<typename T>
    concept FunOpe = requires(T t, unsigned short priorityParent)
    {
        typename T::ValueType;
        T::name();
        t.arity();
        T::priority();
        T::associativity();
        T::symbol();
        T::nbArgBefore();
        t.toString(priorityParent);
    };

    template<typename T>
    concept BoolType = std::is_same_v<T, bool>;
    template<typename T>
    concept ArithmeticType = std::is_same_v<T,void> || std::is_same_v<T, UInt> || std::is_same_v<T, Int> || std::is_same_v<T, Real> 
        || std::is_same_v<T, Complex> || std::is_same_v<T,BooleanSet> || std::is_same_v<T,NaturalIntegerSet>;
    template<typename T>
    concept Type = BoolType<T> || ArithmeticType<T>;
    
    template<FunOpe FunType_>
    class Operator;

    template<typename T>
    concept SubRuleProperty = std::is_base_of_v<Operator<typename T::FunType>, T> || std::is_same_v<T, Boolean> || std::is_same_v<T, ConstBoolean>;
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
