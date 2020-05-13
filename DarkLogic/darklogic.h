/*===---  darklogic.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines DarkLogic API
*   Author: BlackWalker
*   Date: may 6 2020
*
*===----------------------------------------------------------------------===*/
#ifndef DARK_LOGIC_H
#define DARK_LOGIC_H
#include "logic_global.h"
#include <vector>
#include <string>
#include <unordered_map>
#include "Utils/action.h"
#include "Utils/rulecontent.h"

namespace N_DarkLogic
{
	class LOGICSHARED_EXPORT DarkLogic
	{
	public:
		//initialize DarkLogic
		static void init(const size_t& nbInstances);

		//create theorem
		static bool makeTheorem(const std::string& name, const std::string& cont);

		//demonstration functions
		static void apply(const Action::Id& actionKey);
		static void apply(const size_t& instanceIdx, const Action::Id& actionKey);
		static bool applyStr(const std::string& actionStr);
		static bool apply(const std::string& _ruleName, const std::vector<Action::Id>& _path = std::vector<Action::Id>());
		static void unapply();
		static void unapply(const size_t& instanceIdx);
		static const std::vector<Action::Id>& getActions();
		static const std::vector<Action::Id>& getActions(const size_t& instanceIdx);
		static std::vector<Action> getHumanActions();
		static std::unordered_map<std::string, RuleContent> getRuleContents();

		//get information on theorem state
		static bool isOver();
		static bool isOver(const size_t& instanceIdx);
		static bool isDemonstrated();
		static bool isDemonstrated(const size_t& instanceIdx);
		static bool isAlreadyPlayed();
		static bool isAlreadyPlayed(const size_t& instanceIdx);
		static bool hasAlreadyPlayed();
		static bool hasAlreadyPlayed(const size_t& instanceIdx);
		static bool canBeDemonstrated();
		static bool canBeDemonstrated(const size_t& instanceIdx);
		static bool evaluate();
		static bool evaluate(const size_t& instanceIdx);
		static bool isEvaluated();
		static bool isEvaluated(const size_t& instanceIdx);
		static bool appliedRuleSymetric();
		static bool appliedRuleSymetric(const size_t& instanceIdx);

		//get information on theorem content
		static std::string theoremName();
		static void printTheorem();
		static void printTheorem(const size_t& instanceIdx);
		static std::string toStrTheorem();
		static std::string toStrTheorem(const size_t& instanceIdx);

		//clear methods when the demonstration is over
		static void clearAll();
		static void clear();
		static void clear(const size_t& instanceIdx);
	};
}

#endif
