#ifndef DBRULE_H
#define DBRULE_H
#include <unordered_map>
#include "Utils/utils.h"

namespace N_Logic {

template<SubRuleProperty SubPropertyType>
class Rule;
class Action;

class DbRule
{
public:
    DbRule();

    template<RuleType ruleType>
    void insert(const ptr<ruleType>& rule);


    std::pair<ptr<ASubTheorem>,bool> apply(const size_t& actionKey, const ptr<ASubTheorem> &theorem);
    void unapply(const ptr<ASubTheorem> &prop, size_t &nbActions);
    std::vector<size_t> getActions(const ptr<ASubTheorem> &prop, size_t& nbActions);
    std::vector<Action> getHumanActions() const;
    std::shared_ptr<Action> getHumanAction(const size_t& actionKey);
    bool isLastRuleSymetric(const size_t& actionKey) const;

    ~DbRule() = default;
private:
    std::vector<ptr<ASubRule>> m_db;
    std::unique_ptr<size_t> m_nbActions;
    std::vector<size_t> m_actions;
    std::unordered_map<size_t,ptr<ASubRule>> m_actionKeyToRule;
};

template<RuleType ruleType>
void DbRule::insert(const ptr<ruleType >& rule)
{
    if constexpr (std::is_same_v<typename ruleType::SubPropertyType, Equivalent<ASubRule>>)
    {
        m_db.push_back(rule);
        m_db.push_back(rule->getReciprocal());
    }
    else
    {
        m_db.push_back(rule);
    }
    m_db.push_back(rule->getTrueEquivalent());
}
}
#endif // DBRULE_H
