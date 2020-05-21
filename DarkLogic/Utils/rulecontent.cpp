/*===--- rulecontent.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements RuleContent class for DarkLogic Library
*   Author: BlackWalker
*   Date: may 14 2020
*
*===----------------------------------------------------------------------===*/

#include "rulecontent.h"

using namespace N_DarkLogic;

RuleContent::RuleContent(const std::string& name_, const std::string& content_):
	m_name(name_), m_content(content_)
{

}

std::string RuleContent::name() const
{
	return m_name;
}

std::string N_DarkLogic::RuleContent::content() const
{
	return m_content;
}
