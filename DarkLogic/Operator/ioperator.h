/*===--- ioperator.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines operator interface which is base class for operators in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_IOPERATOR_H
#define DARK_LOGIC_IOPERATOR_H
#include <cstddef>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "Utils/utils.h"

namespace N_DarkLogic {

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
#endif // DARK_LOGIC_IOPERATOR_H
