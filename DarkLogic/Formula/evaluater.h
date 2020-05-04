/*===--- evaluater.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines Evaluater class which evaluates SubTheorems in DarkLogic Library
*   Author: BlackWalker
*   Date: april 30 2020
*
*===----------------------------------------------------------------------===*/
#ifndef DARK_LOGIC_EVALUATER_H
#define DARK_LOGIC_EVALUATER_H
#include "Utils/utils.h"

namespace N_DarkLogic
{
	class Evaluater
	{
	public:
		using ConfigEval = std::map<IDVar, bool>;

		Evaluater();
		Evaluater(const char* exceptMsg);
		Evaluater(bool value);

		void setValue(bool value);
		void setHiddenValue(bool value);
		void setExceptMsg(const char* exceptMsg);

		bool isEvaluated() const;
		bool canBeDemonstrated() const;
		bool evaluate() const;
		bool getHiddenValue() const;

		void pushEvalConfig(const ConfigEval& configEval, bool val);
		bool evalWithConfig(const ConfigEval& configEval);
		const std::vector<std::pair<ConfigEval, bool>>& getConfigEvals() const;
		const std::unordered_map<IDVar, IDVar>& getVarToEval() const;

		std::vector<ConfigEval> getCompatibleConfigs(const ConfigEval& commonConfig,
			const std::unordered_map<IDVar, IDVar>& internalVars) const;
		static std::vector<ConfigEval> getCompatibleConfigs(const std::map<ConfigEval, bool>& evalHash,
			const ConfigEval& commonConfig,
			const std::unordered_map<IDVar, IDVar>& internalVars);
		static std::vector<ConfigEval> getAllPossibleConfigs(const std::vector<IDVar>& internalVars);

		void endEval();
		static std::string configToString(const ConfigEval& configEval);
	private:

		//static ConfigEval stringToConfig(const std::string& configStr);

		std::unique_ptr<bool> m_value; //value of SubTheorem (without testing all values of its variables)
		bool m_canBeDemonstrated; //true if and only if subtheorem can be demonstrated
		std::unique_ptr<bool> m_hiddenValue; //value of SubTheorem (after having tested all value of its variables	

		std::vector<std::pair<ConfigEval, bool>> m_configs;
		std::map<ConfigEval, bool> m_evals;
		std::string m_exceptMsg;
		std::unordered_map<IDVar, IDVar> m_varToEval;
	};
};

#endif // !DARK_LOGIC_EVALUATER_H

