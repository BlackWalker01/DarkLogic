/*===--- evaluater.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements Evaluater class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 30 2020
*
*===----------------------------------------------------------------------===*/

#include "evaluater.h"
#include "Variable/variableexception.h"
#include <cassert>
#include <sstream>

using namespace N_DarkLogic;

Evaluater::Evaluater() :
	m_value(nullptr), m_canBeDemonstrated(true), m_hiddenValue(nullptr), m_exceptMsg("")
{
}

Evaluater::Evaluater(const char* exceptMsg) :
	m_value(nullptr), m_canBeDemonstrated(true), m_hiddenValue(nullptr), m_exceptMsg(exceptMsg)
{
}

Evaluater::Evaluater(bool value) :
	m_value(std::make_unique<bool>(value)), m_canBeDemonstrated(true), m_hiddenValue(std::make_unique<bool>(value))
{
}

void Evaluater::setValue(bool value)
{
	m_value = std::make_unique<bool>(value);
	m_hiddenValue = std::make_unique<bool>(value);
}

void Evaluater::setHiddenValue(bool value)
{
	m_hiddenValue = std::make_unique<bool>(value);
	endEval();
}

void Evaluater::setExceptMsg(const char* exceptMsg)
{
	m_exceptMsg = exceptMsg;
}

bool Evaluater::isEvaluated() const
{
	return m_value != nullptr;
}

bool Evaluater::canBeDemonstrated() const
{
	return m_canBeDemonstrated;
}

bool Evaluater::evaluate() const
{
	if (m_value != nullptr)
	{
		return *m_value;
	}
	else
	{
		throw VariableException(m_exceptMsg);
	}
}

bool Evaluater::getHiddenValue() const
{
	return *m_hiddenValue;
}

void Evaluater::pushEvalConfig(const ConfigEval& configEval, bool val)
{
	if (m_evals.find(configEval) == m_evals.end())
	{
		//register variables
		for (const auto& it : configEval)
		{
			m_varToEval[it.first] = it.first;
		}

		m_evals[configEval] = val;
		m_configs.push_back({ configEval,val });
		if (m_canBeDemonstrated)
		{
			if (m_hiddenValue == nullptr)
			{
				m_hiddenValue = std::make_unique<bool>(val);
			}
			else
			{
				if (*m_hiddenValue != val)
				{
					m_hiddenValue = nullptr;
					m_canBeDemonstrated = false;
				}
			}
		}
	}

}

bool Evaluater::evalWithConfig(const ConfigEval& configEval)
{
	if (m_canBeDemonstrated)
	{
		return *m_hiddenValue;
	}
	else
	{
		return m_evals[configEval];
	}
}

const std::vector<std::pair<Evaluater::ConfigEval, bool>>& Evaluater::getConfigEvals() const
{
	return m_configs;
}

const std::unordered_map<IDVar, IDVar>& Evaluater::getVarToEval() const
{
	return m_varToEval;
}

std::vector<Evaluater::ConfigEval> Evaluater::getCompatibleConfigs(const ConfigEval& commonConfig,
	const std::unordered_map<IDVar, IDVar>& internalVars) const
{
	return getCompatibleConfigs(m_evals, commonConfig, internalVars);
}

std::vector<Evaluater::ConfigEval> Evaluater::getCompatibleConfigs(const std::map<ConfigEval, bool>& evalHash,
	const ConfigEval& commonConfig,
	const std::unordered_map<IDVar, IDVar>& internalVars)
{
	if (internalVars.size() == 0)
	{
		return { commonConfig };
	}
	else
	{
		std::vector<ConfigEval> ret;

		std::vector<IDVar> vars;
		for (auto& it : internalVars)
		{
			vars.push_back(it.first);
		}

		std::vector<ConfigEval> subRet = getAllPossibleConfigs(vars);
		for (auto& subConfig : subRet)
		{
			ConfigEval config = commonConfig;
			config.merge(subConfig);
			if (evalHash.find(config) != evalHash.end())
			{
				ret.push_back(config);
			}
		}

		return ret;
	}
}

std::vector<Evaluater::ConfigEval> Evaluater::getAllPossibleConfigs(const std::vector<IDVar>& internalVars)
{
	std::vector<Evaluater::ConfigEval> ret;
	if (internalVars.size())
	{
		auto vars = internalVars;
		IDVar idVar = vars[0];
		vars.erase(vars.begin());
		std::vector<ConfigEval> subRet = getAllPossibleConfigs(vars);

		if (subRet.size())
		{
			//add false value for idVar
			for (auto& config : subRet)
			{
				ConfigEval newConfig;
				newConfig[idVar] = false;
				newConfig.merge(config);
				ret.push_back(newConfig);
			}

			//add true value for idVar
			for (auto& config : subRet)
			{
				ConfigEval newConfig;
				newConfig[idVar] = true;
				newConfig.merge(config);
				ret.push_back(newConfig);
			}
		}
		else
		{
			ConfigEval newConfig;
			newConfig[idVar] = false;
			ret.push_back(newConfig);

			ConfigEval newConfig2;
			newConfig[idVar] = true;
			ret.push_back(newConfig);
		}

	}

	return ret;
}

void Evaluater::endEval()
{
	if (m_canBeDemonstrated)
	{
		assert(m_hiddenValue != nullptr);
		m_evals.clear();
		m_configs.clear();
		m_varToEval.clear();
	}
}

/*
std::string N_DarkLogic::Evaluater::configToString(const ConfigEval& configEval)
{
	std::string ret = "{";
	std::stringstream ss;
	for (const auto& it : configEval)
	{
		std::string first;
		std::string second;
		ret += "{";
		ss << it.first;
		ss >> first;		
		ret += first;
		ss.clear();
		ret += ",";

		ss << it.second;
		ss >> second;
		ret += second;
		ss.clear();

		ret += "}";
		ret += ",";
	}
	ret += "}";
	return ret;
}

Evaluater::ConfigEval N_DarkLogic::Evaluater::stringToConfig(const std::string& configStr)
{
	return ConfigEval();
}*/
