#include "dbrule.h"
#include "dbrule.h"
#include "theorem.h"
#include "Logger/log.h"
using namespace N_Logic;

DbRule::DbRule():
    m_nbActions(std::make_unique<size_t>(0))
{
}

std::pair<ptr<ASubTheorem>,bool> DbRule::apply(const size_t& actionKey, const ptr<ASubTheorem>& theorem)
{
    auto rule=m_actionKeyToRule[actionKey];
    //clear last state
    m_actionKeyToRule.clear();
    m_actions.clear();
    return rule->apply(actionKey,theorem);
}

void DbRule::unapply(const ptr<ASubTheorem> &prop, size_t &nbActions)
{
    for(auto& rule: m_db)
    {
        rule->unapply();
    }    
    (*m_nbActions)--;

    //clear last state
    m_actionKeyToRule.clear();
    m_actions.clear();
    
    getActions(prop,nbActions);
}

std::vector<size_t> DbRule::getActions(const ptr<ASubTheorem> &prop, size_t &/*nbActions*/)
{
    if (!m_actions.size())
    {       
        size_t lastActionIndex = 0;
        for (auto& rule : m_db)
        {
            std::vector<size_t> crtActions = rule->getActions(prop, lastActionIndex);
            for (auto action : crtActions)
            {
                m_actionKeyToRule[action] = rule;
            }
            m_actions.insert(m_actions.end(), crtActions.begin(), crtActions.end());
        }
        (*m_nbActions)++;
    }    
    return m_actions;
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