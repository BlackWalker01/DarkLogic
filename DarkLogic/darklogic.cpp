#include "darklogic.h"
#include "darklogic.h"
#include "darklogic.h"
#include "darklogic.h"
#include "logic.h"

using namespace N_DarkLogic;

void N_DarkLogic::DarkLogic::init(const size_t& nbInstances)
{
	Logic::init(nbInstances);
}

bool N_DarkLogic::DarkLogic::isOver()
{
	return Logic::isOver();
}

bool N_DarkLogic::DarkLogic::isOver(const size_t& instanceIdx)
{
	return Logic::isOver(instanceIdx);
}

bool N_DarkLogic::DarkLogic::isDemonstrated()
{
	return Logic::isDemonstrated();
}

void N_DarkLogic::DarkLogic::clearAll()
{
	Logic::clearAll();
}

void N_DarkLogic::DarkLogic::clear()
{
	return Logic::clear();
}

void N_DarkLogic::DarkLogic::clear(const size_t& instanceIdx)
{
	Logic::clear(instanceIdx);
}

bool N_DarkLogic::DarkLogic::isDemonstrated(const size_t& instanceIdx)
{
	return Logic::isDemonstrated(instanceIdx);
}

bool N_DarkLogic::DarkLogic::isAlreadyPlayed()
{
	return Logic::isAlreadyPlayed();
}

bool N_DarkLogic::DarkLogic::isAlreadyPlayed(const size_t& instanceIdx)
{
	return Logic::isAlreadyPlayed(instanceIdx);
}

bool N_DarkLogic::DarkLogic::hasAlreadyPlayed()
{
	return Logic::hasAlreadyPlayed();
}

bool N_DarkLogic::DarkLogic::canBeDemonstrated(const size_t& instanceIdx)
{
	return Logic::canBeDemonstrated(instanceIdx);
}

bool N_DarkLogic::DarkLogic::evaluate()
{
	return Logic::evaluate();
}

bool N_DarkLogic::DarkLogic::evaluate(const size_t& instanceIdx)
{
	return Logic::evaluate(instanceIdx);
}

bool N_DarkLogic::DarkLogic::isEvaluated()
{
	return Logic::isEvaluated();
}

bool N_DarkLogic::DarkLogic::isEvaluated(const size_t& instanceIdx)
{
	return Logic::isEvaluated(instanceIdx);
}

bool N_DarkLogic::DarkLogic::appliedRuleSymetric()
{
	return Logic::appliedRuleSymetric();
}

bool N_DarkLogic::DarkLogic::appliedRuleSymetric(const size_t& instanceIdx)
{
	return Logic::appliedRuleSymetric(instanceIdx);
}

bool N_DarkLogic::DarkLogic::makeTheorem(const std::string& name, const std::string& cont)
{
	return Logic::makeTheorem(name,cont);
}

void N_DarkLogic::DarkLogic::printTheorem(const size_t& instanceIdx)
{
	Logic::printTheorem(instanceIdx);
}

std::string N_DarkLogic::DarkLogic::toStrTheorem()
{
	return Logic::toStrTheorem();
}

std::string N_DarkLogic::DarkLogic::toStrTheorem(const size_t& instanceIdx)
{
	return Logic::toStrTheorem(instanceIdx);
}

std::string N_DarkLogic::DarkLogic::theoremName()
{
	return Logic::theoremName();
}

void N_DarkLogic::DarkLogic::printTheorem()
{
	return Logic::printTheorem();
}

const std::vector<Action::Id>& N_DarkLogic::DarkLogic::getActions(const size_t& instanceIdx)
{
	return Logic::getActions(instanceIdx);
}

std::vector<Action>N_DarkLogic::DarkLogic::getHumanActions()
{
	return Logic::getHumanActions();
}

std::unordered_map<std::string, RuleContent> N_DarkLogic::DarkLogic::getRuleContents()
{
	return Logic::getRuleContents();
}

void N_DarkLogic::DarkLogic::apply(const Action::Id& actionKey)
{
	return Logic::apply(actionKey);
}

void N_DarkLogic::DarkLogic::apply(const size_t& instanceIdx, const Action::Id& actionKey)
{
	return Logic::apply(instanceIdx,actionKey);
}

bool N_DarkLogic::DarkLogic::applyStr(const std::string& actionStr)
{
	return Logic::apply(actionStr);
}

bool N_DarkLogic::DarkLogic::apply(const std::string& _ruleName, const std::vector<Action::Id>& _path)
{
	return Logic::apply(_ruleName, _path);
}

void N_DarkLogic::DarkLogic::unapply()
{
	Logic::unapply();
}

void N_DarkLogic::DarkLogic::unapply(const size_t& instanceIdx)
{
	return Logic::unapply(instanceIdx);
}

const std::vector<Action::Id>& N_DarkLogic::DarkLogic::getActions()
{
	return Logic::getActions();
}

bool N_DarkLogic::DarkLogic::hasAlreadyPlayed(const size_t& instanceIdx)
{
	return Logic::hasAlreadyPlayed(instanceIdx);
}

bool N_DarkLogic::DarkLogic::canBeDemonstrated()
{
	return Logic::canBeDemonstrated();
}
