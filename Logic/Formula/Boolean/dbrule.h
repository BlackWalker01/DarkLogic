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
    using ActionList = std::vector<size_t>;
    using ActionToRuleMap = std::unordered_map<size_t, ptr<ASubRule>>;

    DbRule();

    template<RuleType ruleType>
    void insert(const ptr<ruleType>& rule);


    std::pair<ptr<ASubTheorem>,bool> apply(const size_t& actionKey, const ptr<ASubTheorem> &theorem);
    void unapply(const ptr<ASubTheorem> &prop);
    const std::vector<size_t>& getActions(const ptr<ASubTheorem> &prop);
    std::vector<Action> getHumanActions() const;
    std::shared_ptr<Action> getHumanAction(const size_t& actionKey);
    bool isLastRuleSymetric(const size_t& actionKey) const;

    void clear();

    ~DbRule() = default;
private:
    bool _insertSafely(const ptr<ASubRule>& rule);
    std::vector<ptr<ASubRule>> m_db;

    ActionList m_actions;
    std::unique_ptr<size_t> m_nbGetActionCalls;
    std::vector<std::pair<ActionList,ActionToRuleMap>> m_oldActions;
    ActionToRuleMap m_actionKeyToRule;
};

template<RuleType ruleType>
void DbRule::insert(const ptr<ruleType >& rule)
{
    if (_insertSafely(rule))
    {
        auto equivalentRules = rule->getEquivalentRules();

        for (const auto& eqRule : equivalentRules)
        {
            _insertSafely(eqRule);
        }

        _insertSafely(rule->getTrueEquivalent());
    }    
}
}
#endif // DBRULE_H
