#pragma once
#include <unordered_map>
#include <map>
#include "Utils/action.h"

class DbAction
{
public:
	using Id = N_DarkLogic::Action::Id;
	using Val = unsigned short int;
	DbAction() = default;

	Id push(Id action);
	void updateValue(Id actionIdx, Val value);
	void removeIdx(Id actionIdx);
	std::vector<Id> actions() const;
	Id getBestAction() const;
	size_t size() const;
	void clear();

private:
	std::unordered_map<Id, Id> m_idxToActions;
	std::map<Val, std::vector<Id>> m_valToActions;
	std::unordered_map<Id, Val> m_idxToVal;
};