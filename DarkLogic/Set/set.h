/*===--- set.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines sets in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SET_H
#define DARK_LOGIC_SET_H
#include <string>
#include <memory>
#include <concepts>
#include "Utils/utils.h"
#include "Formula/Boolean/dbrule.h"

namespace N_DarkLogic {

class DbRule;

template<SubRuleProperty SubPropertyType>
class Rule;

template<typename SetType_>
class AxiomSet;

template<typename SetType_>
class Set
{
public:
    Set();

    static std::string name();
    static bool belongsTo();
    template<RuleType ruleType>
    void insert(const ptr<ruleType>& rule);

    constexpr bool operator==(const Set&) const
    {
        return true;
    }
private:
    static const std::string s_name;
    //static const std::unique_ptr<AxiomSet<SetType_>> s_axiomSet;
    static const std::unique_ptr<DbRule> s_rules;
};

template<typename SetType_>
std::ostream& operator<< (std::ostream& os, const Set<SetType_>& set);

template<typename SetType_>
Set<SetType_>::Set()
{

}

template<typename SetType_>
bool Set<SetType_>::belongsTo()
{
    return true;
}

template<typename SetType_>
std::ostream& operator<< (std::ostream& os, const Set<SetType_>& set)
{
    os<<set.name();
    return os;
}

}
#endif // DARK_LOGIC_SET_H
