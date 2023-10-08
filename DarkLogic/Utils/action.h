/*===--- action.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines actions to transform theorems in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_ACTION_H
#define DARK_LOGIC_ACTION_H
#include <string>
#include <vector>
#include "logic_global.h"

namespace N_DarkLogic
{
class LOGICSHARED_EXPORT Action
{
public:
    using Id = unsigned short int;
    Action(const Id& _id, const std::string& _ruleName, const std::string& _ruleStr, const std::vector<Id>& _path, const std::string& policy="");

    Id id() const;
    std::string ruleName() const;
    std::string ruleStr() const;
    std::vector<Id> path() const;
    std::string policy() const;
    std::string toString() const;

private:
    Id m_id;
    std::string m_ruleName;
    std::string m_ruleStr;
    std::vector<Id> m_path;
    std::string m_policy;
};
}
#endif // DARK_LOGIC_ACTION_H
