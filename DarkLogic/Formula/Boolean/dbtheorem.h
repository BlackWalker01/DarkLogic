/*===--- dbtheorem.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines DbTheorem class which is a container for Rules in DarkLogic Library
*   Author: BlackWalker
*   Date: may 8 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_DBTHEOREM_H
#define DARK_LOGIC_DBTHEOREM_H
#include <unordered_map>
#include "Utils/utils.h"
#include "Operator/Boolean/hyp.h"
#include "Variable/NaturalInteger/constnaturalinteger.h"

namespace N_DarkLogic
{
class IISubTheoremFormula;

template<SubTheoremProperty SubPropertyType>
class SubTheorem;
template<ArithConstantType VarType>
class ConstSubArithmeticTheorem;
class ConstNaturalInteger;

class DbTheorem
{
public:
	DbTheorem() = default;

	template<typename Type, typename SubArg>
	ptr<Type> make_theorem_formula(SubArg&& subArg);

	template<typename Type, typename LeftArg, typename RightArg>
	ptr<Type> make_theorem_formula(LeftArg&& leftArg, RightArg&& rightArg);	

	void clear();
	size_t size() const;

	~DbTheorem() = default;
private:
	std::unordered_map<std::string, ptr<IISubTheoremFormula>> m_db;
	
};

template<typename Type, typename SubArg>
inline ptr<Type> DbTheorem::make_theorem_formula(SubArg&& subArg)
{
	if constexpr (std::is_base_of_v<SubTheorem<ConstBoolean>, Type>)
	{
		std::string cont = subArg ? "True" : "False";
		auto it = m_db.find(cont);
		if (it != m_db.end())
		{
			auto ret = std::dynamic_pointer_cast<const Type>(it->second);
			if (ret)
			{
				return ret;
			}
		}
		auto ret = std::make_shared<const Type>(subArg);
		m_db[cont] = ret;
		return ret;
	}
	else if constexpr (std::is_base_of_v<SubTheorem<Hyp<ASubTheorem>>, Type>)
	{
		using Fun = typename Type::SubPropertyType::FunType;
		std::string cont = "{";
		size_t k = 0;
		cont += subArg[k]->toString();
		for (k = 1; k < subArg.size() - 2; k++)
		{
			cont += "," + subArg[k]->toString();
		}
		if (subArg.size() > 2)
		{
			cont += "," + subArg[k]->toString();
		}
		cont += "} ";
		cont += subArg.back()->toString(Fun::priority());

		auto it = m_db.find(cont);
		if (it != m_db.end())
		{
			auto ret = std::dynamic_pointer_cast<const Type>(it->second);
			if (ret)
			{
				return ret;
			}
		}
		auto ret = std::make_shared<const Type>(subArg);
		m_db[cont] = ret;
		return ret;
	}
	else if constexpr (std::is_base_of_v<ConstSubArithmeticTheorem<ConstNaturalInteger>, Type>)
	{
		std::string cont = ConstNaturalInteger::valToString(subArg);
		auto it = m_db.find(cont);
		if (it != m_db.end())
		{
			auto ret = std::dynamic_pointer_cast<const ConstSubArithmeticTheorem<ConstNaturalInteger>>(it->second);
			if (ret)
			{
				return ret;
			}
		}
		auto ret = std::make_shared<const ConstSubArithmeticTheorem<ConstNaturalInteger>>(subArg);
		m_db[cont] = ret;
		return ret;
	}
	else
	{
		using Fun = typename Type::SubPropertyType::FunType;
		std::string cont = Fun::symbol() + subArg->toString();
		auto it = m_db.find(cont);
		if (it != m_db.end())
		{
			auto ret = std::dynamic_pointer_cast<const Type>(it->second);
			if (ret)
			{
				return ret;
			}
		}
		auto ret = std::make_shared<const Type>(subArg);
		m_db[cont] = ret;
		return ret;
	}
	
}
template<typename Type, typename LeftArg, typename RightArg>
inline ptr<Type> DbTheorem::make_theorem_formula(LeftArg&& leftArg, RightArg&& rightArg)
{
	using Fun = typename Type::SubPropertyType::FunType;
	std::string cont = leftArg->toString() + Fun::symbol() + rightArg->toString();
	auto it = m_db.find(cont);
	if (it != m_db.end())
	{
		auto ret = std::dynamic_pointer_cast<const Type>(it->second);
		if (ret)
		{
			return ret;
		}
	}
	auto ret = std::make_shared<const Type>(leftArg, rightArg);
	m_db[cont] = ret;
	return ret;
}
}


#endif
