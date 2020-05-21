/*===--- rulecontent.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines Rule Contents for DarkLogic Library
*   Author: BlackWalker
*   Date: may 14 2020
*
*===----------------------------------------------------------------------===*/
#ifndef DARK_LOGIC_RULECONTENT_H
#define DARK_LOGIC_RULECONTENT_H
#include "logic_global.h"
#include <string>

namespace N_DarkLogic
{
class LOGICSHARED_EXPORT RuleContent
{
public:
	RuleContent() = default;
	RuleContent(const std::string& name_, const std::string& content_);

	std::string name() const;
	std::string content() const;

private:
	std::string m_name;
	std::string m_content;
};
}
#endif // !DARK_LOGIC_RULECONTENT_H