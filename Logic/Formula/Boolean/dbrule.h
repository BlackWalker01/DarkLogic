#ifndef DBRULE_H
#define DBRULE_H
#include <unordered_map>
#include "rule.h"

namespace N_Logic {
class DbRule
{
public:
    DbRule() = default;

    template<typename OpeType>
    void insert(const ptr<Rule<OpeType>>& rule);


    ptr<ASubTheorem> apply(const size_t& actionKey, const ptr<ASubTheorem> &theorem);
    void unapply(const ptr<ASubTheorem> &prop, size_t &nbActions);
    std::vector<size_t> getActions(const ptr<ASubTheorem> &prop, size_t& nbActions);
    std::vector<Action> getHumanActions() const;

    ~DbRule() = default;
private:
    std::vector<ptr<ASubRule>> m_db;
    std::unordered_map<size_t,ptr<ASubRule>> m_actionKeyToRule;
};

template<typename OpeType>
void DbRule::insert(const ptr<Rule<OpeType> > &rule)
{
    m_db.push_back(rule);
}
}
#endif // DBRULE_H
