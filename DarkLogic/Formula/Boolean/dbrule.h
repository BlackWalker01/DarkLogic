/*===--- dbrule.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines DbRule class which is a container for Rules in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_DBRULE_H
#define DARK_LOGIC_DBRULE_H
#include <unordered_map>
#include "Utils/utils.h"
#include "Utils/action.h"
#include "Utils/rulecontent.h"

namespace N_DarkLogic {

template<SubRuleProperty SubPropertyType>
class Rule;
class Action;

class DbRule
{
public:
    using ActionList = std::vector<Action::Id>;
    using ActionToRuleMap = std::unordered_map<Action::Id, ptr<ASubRule>>;

    DbRule();

    template<RuleType ruleType>
    void insert(const ptr<ruleType>& rule);


    //Automatic methods
    std::pair<ptr<ASubTheorem>,bool> apply(const Action::Id& actionKey);
    void unapply();
    const std::vector<Action::Id>& getActions(const ptr<ASubTheorem>& prop);
    const std::vector<Action::Id>& getActions(const ptr<ASubTheorem> &prop, const size_t& logicIdx);    
    bool isLastRuleSymetric(const Action::Id& actionKey) const;

    //Human methods
    std::vector<Action> getHumanActions(const ptr<ASubTheorem>& prop);
    std::shared_ptr<Action> getHumanAction(const ptr<ASubTheorem>& prop, const Action::Id& actionKey);
    std::unordered_map<std::string, RuleContent> getRuleContents() const;
    std::tuple<ptr<ASubTheorem>, bool, Action::Id> apply(const ptr<ASubTheorem>& prop, const std::string& _ruleName, 
        const std::vector<Action::Id>& _path);

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
#endif // DARK_LOGIC_DBRULE_H
