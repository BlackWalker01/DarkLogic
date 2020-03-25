#ifndef DBRULE_H
#define DBRULE_H
#include <unordered_map>
#include "rule.h"

namespace N_Logic {
class DbRule
{
public:
    DbRule();

    template<typename OpeType>
    void insert(const ptr<Rule<OpeType>>& rule);


    std::pair<ptr<ASubTheorem>,bool> apply(const size_t& actionKey, const ptr<ASubTheorem> &theorem);
    void unapply(const ptr<ASubTheorem> &prop, size_t &nbActions);
    std::vector<size_t> getActions(const ptr<ASubTheorem> &prop, size_t& nbActions);
    std::vector<Action> getHumanActions() const;
    bool isLastRuleSymetric(const size_t& actionKey) const;

    ~DbRule() = default;
private:
    std::vector<ptr<ASubRule>> m_db;
    std::unique_ptr<size_t> m_nbActions;
    std::vector<size_t> m_actions;
    std::unordered_map<size_t,ptr<ASubRule>> m_actionKeyToRule;
};

template<typename OpeType>
void DbRule::insert(const ptr<Rule<OpeType> > &rule)
{
    if constexpr (std::is_same_v<OpeType, Equivalent<ASubRule>>)
    {
        m_db.push_back(rule);
        m_db.push_back(rule->getReciprocal());
    }
    else
    {
        m_db.push_back(rule);
    }    
}
}
#endif // DBRULE_H
