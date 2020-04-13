#include "avariable.h"

using namespace N_DarkLogic;
std::unordered_map<std::string,IDVar> AVariable::s_nameToID = {};

N_DarkLogic::AVariable::AVariable(const std::string& name_): AbstractTerm(name_), m_id(getIDVar(name_))
{
}

IDVar N_DarkLogic::AVariable::id() const
{
	return m_id;
}

std::vector<std::string> N_DarkLogic::AVariable::getAllVarName()
{
	std::vector<std::string> ret;
	for (const auto& it : s_nameToID)
	{
		ret.push_back(it.first);
	}
	return ret;
}

IDVar N_DarkLogic::AVariable::getIDVar(const std::string& name_)
{
	auto it = s_nameToID.find(name_);
	if (it != s_nameToID.end())
	{
		return it->second;
	}
	else
	{
		IDVar ret = s_nameToID.size();
		s_nameToID[name_] = ret;
		return ret;
	}
}
