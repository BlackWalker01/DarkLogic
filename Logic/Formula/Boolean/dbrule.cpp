#include "dbrule.h"
#include "theorem.h"
#include "Logger/log.h"
using namespace N_Logic;

ptr<ASubTheorem> DbRule::apply(const size_t& actionKey, const ptr<ASubTheorem>& theorem)
{
    auto rule=m_actionKeyToRule[actionKey];
    Log::Debug("Apply rule "+rule->toString());
    return rule->apply(actionKey,theorem);
}

void DbRule::unapply(const ptr<ASubTheorem> &prop, size_t &nbActions)
{
    for(auto& rule: m_db)
    {
        rule->unapply();
    }
    m_actionKeyToRule.clear();

    getActions(prop,nbActions);
}

std::vector<size_t> DbRule::getActions(const ptr<ASubTheorem> &prop, size_t &nbActions)
{
    std::vector<size_t> ret;
    size_t lastActionIndex=0;
    for(auto& rule: m_db)
    {
        std::vector<size_t> crtActions=rule->getActions(prop,nbActions,lastActionIndex);
        for(auto action: crtActions)
        {
            m_actionKeyToRule[action]=rule;
        }
        ret.insert(ret.end(),crtActions.begin(),crtActions.end());
    }
    return ret;
}

std::vector<Action> DbRule::getHumanActions() const
{
    std::vector<Action> ret;
    for(const auto& rule: m_db)
    {
        std::vector<Action> actions=rule->getHumanActions();
        ret.insert(ret.end(),actions.begin(),actions.end());
    }
    return ret;
}

bool DbRule::isLastRuleSymetric(const size_t& actionKey) const
{
    return m_actionKeyToRule.at(actionKey)->isSymetric();
}