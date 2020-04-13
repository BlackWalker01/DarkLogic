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
    Action(const size_t& _id, const std::string& _ruleName, const std::string& _ruleStr, const std::vector<size_t>& _path);

    size_t id() const;
    std::string ruleName() const;
    std::string ruleStr() const;
    std::vector<size_t> path() const;
    std::string toString() const;

private:
    size_t m_id;
    std::string m_ruleName;
    std::string m_ruleStr;
    std::vector<size_t> m_path;
};
}
#endif // DARK_LOGIC_ACTION_H
