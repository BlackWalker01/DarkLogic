#include "dbrule.h"
#include "dbrule.h"
#include "theorem.h"
#include "Logger/log.h"
#include "rule.h"
#include "Utils/action.h"

using namespace N_Logic;

DbRule::DbRule(): m_nbGetActionCalls(std::make_unique<size_t>(0))
{
}

std::pair<ptr<ASubTheorem>,bool> DbRule::apply(const size_t& actionKey, const ptr<ASubTheorem>& theorem)
{
    auto rule=m_actionKeyToRule[actionKey];

    //store last actions
    m_oldActions.push_back({ m_actions,m_actionKeyToRule });
    for (const auto& rule : m_db)
    {
        rule->storeActions();
    }

    //clear last state
    m_actionKeyToRule.clear();
    m_actions.clear();

    //apply rule
    return rule->apply(actionKey,theorem);
}

void DbRule::unapply(const ptr<ASubTheorem> &prop)
{
    for(auto& rule: m_db)
    {
        rule->unstoreActions();
    }    

    //get last state    
    m_actions = m_oldActions.back().first;
    m_actionKeyToRule = m_oldActions.back().second;
    m_oldActions.pop_back();
    if (m_oldActions.size() + 2 == (*m_nbGetActionCalls))
    {
        (*m_nbGetActionCalls)--;
    }
}

const std::vector<size_t>& DbRule::getActions(const ptr<ASubTheorem> &prop)
{
    if ((*m_nbGetActionCalls) == m_oldActions.size())
    {       
        m_actions.clear();
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
        (*m_nbGetActionCalls)++;
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

std::shared_ptr<Action> N_Logic::DbRule::getHumanAction(const size_t& actionKey)
{
    for (const auto& rule : m_db)
    {
        std::vector<Action> actions = rule->getHumanActions();
        for (auto& action : actions)
        {
            if (action.id() == actionKey)
            {
                return std::make_shared<Action>(action);
            }
        }
    }
    return nullptr;
}

bool DbRule::isLastRuleSymetric(const size_t& actionKey) const
{
    return m_actionKeyToRule.at(actionKey)->isSymetric();
}