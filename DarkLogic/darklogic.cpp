#include "darklogic.h"
#include "logic.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

using namespace N_DarkLogic;

const unsigned int DarkLogic::s_maj = 1;
const unsigned int DarkLogic::s_mid = 4;
const unsigned int DarkLogic::s_min = 1;


PYBIND11_MODULE(DarkLogic, m)
{
	py::class_<DarkLogic> darkLogic(m, "DarkLogic");
	darkLogic.def("init", &DarkLogic::init)
		.def("makeTheorem", py::overload_cast<const std::string&, const std::string&> (&DarkLogic::makeTheorem))
		.def("apply", py::overload_cast<const Action::Id&>(&DarkLogic::apply))
		.def("apply", py::overload_cast<const size_t&, const Action::Id&>(&DarkLogic::apply))
		.def("apply", py::overload_cast<const std::string&, const std::vector<Action::Id>&>(&DarkLogic::apply))
		.def("applyStr", py::overload_cast<const std::string&>(&DarkLogic::applyStr))
		.def("unapply", py::overload_cast<>(&DarkLogic::unapply))
		.def("unapply", py::overload_cast<const size_t&>(&DarkLogic::unapply))
		.def("getActions", py::overload_cast<>(&DarkLogic::getActions))
		.def("getActions", py::overload_cast<const size_t&>(&DarkLogic::getActions))
		.def("getHumanActions", py::overload_cast<>(&DarkLogic::getHumanActions))
		.def("getRuleContents", &DarkLogic::getRuleContents)

		//get information on theorem state
		.def("isOver", py::overload_cast<>(&DarkLogic::isOver))
		.def("isOver", py::overload_cast<const size_t&>(&DarkLogic::isOver))
		.def("isDemonstrated", py::overload_cast<>(&DarkLogic::isDemonstrated))
		.def("isDemonstrated", py::overload_cast<const size_t&>(&DarkLogic::isDemonstrated))
		.def("isAlreadyPlayed", py::overload_cast<>(&DarkLogic::isAlreadyPlayed))
		.def("isAlreadyPlayed", py::overload_cast<const size_t&>(&DarkLogic::isAlreadyPlayed))
		.def("hasAlreadyPlayed", py::overload_cast<>(&DarkLogic::hasAlreadyPlayed))
		.def("hasAlreadyPlayed", py::overload_cast<const size_t&>(&DarkLogic::hasAlreadyPlayed))
		.def("canBeDemonstrated", py::overload_cast<>(&DarkLogic::canBeDemonstrated))
		.def("canBeDemonstrated", py::overload_cast<const size_t&>(&DarkLogic::canBeDemonstrated))
		.def("evaluate", py::overload_cast<>(&DarkLogic::evaluate))
		.def("evaluate", py::overload_cast<const size_t&>(&DarkLogic::evaluate))
		.def("isEvaluated", py::overload_cast<>(&DarkLogic::isEvaluated))
		.def("isEvaluated", py::overload_cast<const size_t&>(&DarkLogic::isEvaluated))
		.def("appliedRuleSymetric", py::overload_cast<>(&DarkLogic::appliedRuleSymetric))
		.def("appliedRuleSymetric", py::overload_cast<const size_t&>(&DarkLogic::appliedRuleSymetric))
		.def("getState", py::overload_cast<>(&DarkLogic::getState))
		.def("getState", py::overload_cast<const size_t&>(&DarkLogic::getState))
		.def("getRuleStates", py::overload_cast<>(&DarkLogic::getRuleStates))

		.def("theoremName", &DarkLogic::theoremName)
		.def("printTheorem", py::overload_cast<>(&DarkLogic::printTheorem))
		.def("printTheorem", py::overload_cast<const size_t&>(&DarkLogic::printTheorem))
		.def("toStrTheorem", py::overload_cast<>(&DarkLogic::toStrTheorem))
		.def("toStrTheorem", py::overload_cast<const size_t&>(&DarkLogic::toStrTheorem))
		.def("toNormStrTheorem", py::overload_cast<>(&DarkLogic::toNormStrTheorem))
		.def("nbTheorems", py::overload_cast<const size_t&>(&DarkLogic::nbTheorems))

		.def("clearAll", &DarkLogic::clearAll)
		.def("clear", py::overload_cast<>(&DarkLogic::clear))
		.def("clear", py::overload_cast<const size_t&>(&DarkLogic::clear))
		.def("version", &DarkLogic::version);

	py::class_<Action> action(m, "Action");
	action.def(py::init<const Action::Id&, const std::string&, const std::string&, const std::vector<Action::Id>&>())
		.def("id", &Action::id)
		.def("ruleName", &Action::ruleName)
		.def("ruleStr", &Action::ruleStr)
		.def("path", &Action::path)
		.def("toString", &Action::toString);

	py::class_<State> state(m, "State");
	state.def("priorityOpe", &State::priorityOpe)
		.def("operators", &State::operators)
		.def("terms", &State::terms);

	py::class_<State::Term> term(m, "Term");
	term.def("isVariable", &State::Term::isVariable)
		.def("type", &State::Term::type)
		.def("id", &State::Term::id)
		.def("val", &State::Term::val);

	py::class_<State::OrderedName> orderedName(m, "OrderedName");
	orderedName.def("name", &State::OrderedName::name)
		.def("parentOperators", &State::OrderedName::parentOperators);

	py::class_<State::ParentOperator> parentOperator(m, "ParentOperator");
	parentOperator.def("name", &State::ParentOperator::name)
		.def("idx", &State::ParentOperator::idx);

	py::enum_<Name> name(m, "name");
	name.value("NONE", Name::NONE)
		.value("AND", Name::AND)
		.value("EQUIVALENT", Name::EQUIVALENT)
		.value("IMPLICATION", Name::IMPLICATION)
		.value("NOT", Name::NOT)
		.value("OR", Name::OR)
		.value("HYP", Name::HYP)
		.value("EQUAL", Name::EQUAL)
		.value("SETEQUAL", Name::SETEQUAL)
		.value("LET", Name::LET)
		.value("BELONGSTO", Name::BELONGSTO)
		.value("PLUS", Name::PLUS);

	py::enum_<VALUE_TYPE> valueType(m, "Value_Type");
	valueType.value("VOID_TYPE", VALUE_TYPE::VOID_TYPE)
		.value("BOOL_TYPE", VALUE_TYPE::BOOL_TYPE)
		.value("NATURAL_INT_TYPE", VALUE_TYPE::NATURAL_INT_TYPE)
		.value("BOOLEANSET_TYPE", VALUE_TYPE::BOOLEANSET_TYPE)
		.value("NATURAL_INTEGERSET_TYPE", VALUE_TYPE::NATURAL_INTEGERSET_TYPE);

	py::enum_<Associativity> associativity(m, "Associativity");
	associativity.value(" RIGHT", Associativity::RIGHT)
		.value("LEFT", Associativity::LEFT);
}



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

const State& N_DarkLogic::DarkLogic::getState()
{
	return Logic::getState();
}

const State& N_DarkLogic::DarkLogic::getState(const size_t& instanceIdx)
{
	return Logic::getState(instanceIdx);
}

std::vector<State> N_DarkLogic::DarkLogic::getRuleStates()
{
	return Logic::getRuleStates();
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

std::string N_DarkLogic::DarkLogic::toNormStrTheorem()
{
	return Logic::toNormStrTheorem();
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

size_t N_DarkLogic::DarkLogic::nbTheorems(const size_t& instanceIdx)
{
	return Logic::nbTheorems(instanceIdx);
}

std::string N_DarkLogic::DarkLogic::version()
{
	return uintToString(s_maj) + "." + uintToString(s_mid) + "." + uintToString(s_min);
}