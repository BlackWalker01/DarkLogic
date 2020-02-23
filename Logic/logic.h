#ifndef LOGIC_H
#define LOGIC_H
#include <unordered_map>
#include "Operator/operator.h"
#include "Formula/Boolean/dbrule.h"
#include <stack>

namespace N_Logic {

class Logic
{
private:
    Logic();

public:
    friend typename std::_MakeUniq<Logic>::__single_object std::make_unique<Logic>();
    static void init();

    //property access methods
    static bool evaluate();
    static bool makeTheorem(const std::string &name, const std::string &cont);
    static void testTheorem();
    static void printTheorem();

    //operator access methods
    static bool isOpeSymbol(std::string symbol);
    static std::vector<size_t> getActions();

    //demonstration methods
    static void apply(const size_t& actionKey);
    static void apply(const std::string& actionKey);
    static void unapply();

    ~Logic() = default;

private:
    ptr<ASubTheorem> m_theorem;
    std::stack<ptr<ASubTheorem>> m_antecedents;
    DbRule m_rules;
    std::unordered_map<std::string,size_t> m_ruleNameToIndex;
    size_t m_nbAppliedRule;

    static std::unique_ptr<Logic> instance;

    template<typename OpeType>
    void insert(const ptr<Rule<OpeType>>& rule);
};
}
#endif // LOGIC_H
