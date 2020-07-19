/*===--- dbrule.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements DbRule class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "dbrule.h"
#include "dbrule.h"
#include "theorem.h"
#include "Logger/log.h"
#include "rule.h"
#include "Utils/action.h"

using namespace N_DarkLogic;

DbRule::DbRule(): m_nbGetActionCalls(std::make_unique<size_t>(0))
{
}

std::pair<ptr<ASubTheorem>,bool> DbRule::apply(const Action::Id& actionKey)
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
    return rule->apply(actionKey);
}

void DbRule::unapply()
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

const std::vector<Action::Id>& N_DarkLogic::DbRule::getActions(const ptr<ASubTheorem>& prop)
{
    if ((*m_nbGetActionCalls) == m_oldActions.size())
    {
        m_actions.clear();
        Action::Id lastActionIndex = 0;
        for (auto& rule : m_db)
        {
            std::vector<Action::Id> crtActions = rule->getActions(prop, lastActionIndex);
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

const std::vector<Action::Id>& DbRule::getActions(const ptr<ASubTheorem> &prop, const size_t& logicIdx)
{
    if ((*m_nbGetActionCalls) == m_oldActions.size())
    {       
        m_actions.clear();
        Action::Id lastActionIndex = 0;
        for (auto& rule : m_db)
        {
            std::vector<Action::Id> crtActions = rule->getActions(prop, lastActionIndex, logicIdx);
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

std::vector<Action> DbRule::getHumanActions(const ptr<ASubTheorem>& prop)
{
    std::vector<Action> ret;
    Action::Id lastActionIndex = 0;
    m_actions.clear();
    for(const auto& rule: m_db)
    {
        std::vector<Action> actions=rule->getHumanActions(prop, lastActionIndex);
        ret.insert(ret.end(),actions.begin(),actions.end());
        for (auto action : actions)
        {
            m_actionKeyToRule[action.id()] = rule;
            m_actions.push_back(action.id());
        }
    }

    if ((*m_nbGetActionCalls) == m_oldActions.size())
    {
        (*m_nbGetActionCalls)++;                
    }

    return ret;
}

std::shared_ptr<Action> N_DarkLogic::DbRule::getHumanAction(const ptr<ASubTheorem>& prop, const Action::Id& actionKey)
{
    Action::Id lastActionIndex = 0;
    for (const auto& rule : m_db)
    {
        std::vector<Action> actions = rule->getHumanActions(prop, lastActionIndex);
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

bool DbRule::isLastRuleSymetric(const Action::Id& actionKey) const
{
    return m_actionKeyToRule.at(actionKey)->isSymetric();
}

std::vector<State> N_DarkLogic::DbRule::getStates() const
{
    std::vector<State> ret;
    for (const auto& rule : m_db)
    {
        ret.push_back(rule->getState());
    }
    return ret;
}

std::unordered_map<std::string, RuleContent> N_DarkLogic::DbRule::getRuleContents() const
{
    std::unordered_map<std::string, RuleContent> ret;
    for (const auto& rule : m_db)
    {
        ret[rule->name()] = RuleContent(rule->name(), rule->toString());
    }
	return ret;
}

std::tuple<ptr<ASubTheorem>, bool, Action::Id> DbRule::apply(const ptr<ASubTheorem>& prop, const std::string& _ruleName,
    const std::vector<Action::Id>& _path)
{
    auto actions = getHumanActions(prop);
    for (const auto& action : actions)
    {
        if ((action.ruleName() == _ruleName) && (action.path() == _path))
        {
            auto couple = apply(action.id());

            //store last actions
            m_oldActions.push_back({ m_actions,m_actionKeyToRule });
            for (const auto& rule : m_db)
            {
                rule->storeActions();
            }

            //clear last state
            m_actionKeyToRule.clear();
            m_actions.clear();

            return { couple.first, couple.second, action.id() };
        }
    }

    auto tabToString = [](const std::vector<Action::Id>& tab)
    {
        std::string ret = "[";
        if (tab.size() > 0)
        {
            for (size_t k = 0; k < tab.size() - 1; k++)
            {
                ret += sizeToString(tab[k]) + ",";
            }
            ret += sizeToString(tab[tab.size() - 1]);
        }
        ret += "]";
        return ret;
    };

    throw std::runtime_error("Action {RuleName: "+_ruleName+", path: "+tabToString(_path)+"}");
}

void N_DarkLogic::DbRule::clear()
{
    m_actions.clear();
    (*m_nbGetActionCalls) = 0;
    m_oldActions.clear();
    m_actionKeyToRule.clear();
    for (const auto& rule : m_db)
    {
        rule->clearAllIdentifications();
    }
}

bool N_DarkLogic::DbRule::_insertSafely(const ptr<ASubRule>& rule)
{    
    auto varNames = AVariable::getAllVarName();
    std::unordered_map<std::string, std::string> varNameDb;
    std::unordered_map<std::string, std::string> varContents;
    for (const auto& varName : varNames)
    {
        std::string ch;
        for (size_t k = 0; k < varName.size(); k++)
        {            
            ch += varName[k];
            varContents[ch] = ch;
        }
        varNameDb[varName] = varName;
    }

    auto isVar = [varNameDb](const std::string& nameVar)
    {
        return varNameDb.find(nameVar) != varNameDb.end();
    };

    auto isStringInVar = [varContents](const std::string& nameVar)
    {
        return varContents.find(nameVar) != varContents.end();
    };

    auto getVarName = [&isVar, &isStringInVar](const std::string& ruleStr)
    {
        std::vector<std::string> ret;
        std::unordered_map<std::string, std::string> dbVar;
        std::string crtVar = "";
        for (size_t k = 0; k < ruleStr.size(); k++)
        {
            crtVar += ruleStr[k];
            if (isVar(crtVar))
            {
                if (dbVar.find(crtVar) == dbVar.end())
                {
                    ret.push_back(crtVar);
                    dbVar[crtVar] = crtVar;
                }
                
                crtVar = "";
            }
            else if (!isStringInVar(crtVar))
            {
                crtVar = "";
            }
        }
        return ret;
    };

    auto replaceVarIn = [&isVar, &isStringInVar](const std::string& ruleStr, const std::unordered_map<std::string, std::string>& varNameAssoc)
    {
        std::string ret;
        std::string crtVar = "";
        for (size_t k = 0; k < ruleStr.size(); k++)
        {
            crtVar += ruleStr[k];
            if (isVar(crtVar))
            {
                ret += varNameAssoc.at(crtVar);
                crtVar = "";
            }
            else if (!isStringInVar(crtVar))
            {
                ret += crtVar;
                crtVar = "";
            }
        }
        return ret;
    };

    std::string ruleStr = rule->toString();
    for (const auto& ruleDb : m_db)
    {                
        std::string ruleDbStr = ruleDb->toString();
        if (ruleStr == ruleDbStr)
        {
            return false;
        }
        auto ruleVarNames = getVarName(ruleStr);
        auto ruleDbVarNames = getVarName(ruleDbStr);
        if ((ruleVarNames.size() == ruleDbVarNames.size()) && (ruleStr.size() == ruleDbStr.size()))
        {
            auto permutationTab = permutation(ruleVarNames.size());
            std::unordered_map<std::string, std::string> varNameAssoc;
            for (const auto& permuter : permutationTab)
            {
                for (size_t k = 0; k < permuter.size(); k++)
                {
                    varNameAssoc[ruleVarNames[k]] = ruleDbVarNames[permuter[k]];
                }

                //replace and compare 
                if(replaceVarIn(ruleStr,varNameAssoc)==ruleDbStr)
                {
                    return false;
                }                
            }
        }
    }
    m_db.push_back(rule);
    return true;
}
