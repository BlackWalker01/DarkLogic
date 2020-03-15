#ifndef SET_H
#define SET_H
#include <string>
#include <memory>
#include "Utils/utils.h"

namespace N_Logic {

class DbRule;

template<typename SubPropertyType>
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
    template<typename SubPropertyType>
    void insert(const ptr<Rule<SubPropertyType>>& rule);

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
std::string Set<SetType_>::name()
{
    return s_name;
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
#endif // SET_H
