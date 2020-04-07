#ifndef IOPERATOR_H
#define IOPERATOR_H
#include <cstddef>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "Utils/utils.h"

namespace N_Logic {

class IOperator
{
public:
    IOperator() = default;
    IOperator(const IOperator& ope) = default;

    virtual Name name() const=0;
    virtual Arity arity() const=0;
    virtual unsigned short priority() const=0;
    virtual Associativity associativity() const=0;
    virtual std::string symbol() const=0;


    static bool isOpeName(const std::string& name);
    static Name getOpeName(const std::string& name_);
    static Name getNextOpeName(const std::string& propCont, size_t& i);
    static void skipStrings(const std::string& propCont, size_t& i);

    virtual ~IOperator() = default;

private:
    static const std::unordered_map<std::string,Name>  s_nameHash;
    static const std::unordered_map<std::string,std::string> s_mightBeOpeName;
    static const std::unordered_map<std::string,std::string> s_stringsToSkip;
    static const std::unordered_map<std::string,std::string> s_stringsMightSkip;

    static std::unordered_map<std::string,std::string> initStringMightSkip();
    static std::unordered_map<std::string,std::string> initMightBeOpeName();

    static bool mustBeSkipped(const std::string& s);
    static bool mightBeSkipped(const std::string& s);
    static bool mightBeOpeName(const std::string& s);
};
}
#endif // IOPERATOR_H
