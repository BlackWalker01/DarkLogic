/*===--- action.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements action class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "action.h"
#include "utils.h"

using namespace N_DarkLogic;

Action::Action(const size_t &_id, const std::string &_ruleName, const std::string &_ruleStr, const std::vector<size_t> &_path):
    m_id(_id), m_ruleName(_ruleName), m_ruleStr(_ruleStr), m_path(_path)
{

}

size_t Action::id() const
{
    return m_id;
}

std::string Action::ruleName() const
{
    return m_ruleName;
}

std::string Action::ruleStr() const
{
    return m_ruleStr;
}

std::vector<size_t> Action::path() const
{
    return m_path;
}

std::string Action::toString() const
{
    auto tabToString=[](const std::vector<size_t>& tab)
    {
        std::string ret="[";
        if(tab.size()>0)
        {
            for(size_t k=0; k<tab.size()-1;k++)
            {
                ret+=sizeToString(tab[k])+",";
            }
            ret+=sizeToString(tab[tab.size()-1]);
        }
        ret+="]";
        return ret;
    };
    return "{id: "+sizeToString(m_id)+", ruleName: "+m_ruleName+", rule:"+m_ruleStr+", path: "+tabToString(m_path)+"}";
}