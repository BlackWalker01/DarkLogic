/*===--- state.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file will  define states of theorems for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_STATE_H
#define DARK_LOGIC_STATE_H
#include "logic_global.h"
#include "Utils/utils.h"
#include <vector>
#include <tuple>
#include "Set/allset.h"

namespace N_DarkLogic
{

class LOGICSHARED_EXPORT State
{
public:
	class LOGICSHARED_EXPORT Term
	{
	public:
		template<typename ValueType>
		Term(VALUE_TYPE type_, ValueType val_);
		Term(IDVar id_, VALUE_TYPE type_);
		Term(const Term& term);

		bool isVariable() const;
		VALUE_TYPE type() const;
		const IDVar& id() const;
		const DLVariant& val() const;

	private:
		bool m_isVariable;
		VALUE_TYPE m_type;
		std::unique_ptr<IDVar> m_id;
		std::unique_ptr<DLVariant> m_val;
	};

	/**
	* Class which encapsulates operators to order them
	*/
	class LOGICSHARED_EXPORT OrderedName
	{
	public:
		OrderedName(Name name_, const unsigned int& nbHyps_, const unsigned int& argIdx_);

		Name name() const; //name enum of current operator
		unsigned int nbHyps() const; //number of hypothesis operators in which current operator is
		unsigned int argIdx() const;
	private:
		Name m_name;
		unsigned int m_nbHyps;
		unsigned int m_argIdx;
	};

	//Constructors
	State(Name headOpe, const State& subState);
	State(Name headOpe, const State& leftState, const State& rightState);
	State(Name headOpe, const std::vector<State>& subStates);
	State(const IDVar& idVar, VALUE_TYPE varType);
	template<typename Value>
	State(VALUE_TYPE varType, const Value& val);

	const std::vector<OrderedName>& priorityOpe() const;
	const std::vector<Name>& operators() const;
	const std::vector<Term>& terms() const;

private:
	const std::vector<OrderedName> m_priorityOpe; //operators ordered by priority
	const std::vector<Name> m_operators; //operators ordered by appearance in string
	const std::vector<Term> m_terms; //list of terms
};
template<typename Value>
inline State::State(VALUE_TYPE varType, const Value& val): m_terms({Term(varType, val)})
{
}
template<typename ValueType>
inline State::Term::Term(VALUE_TYPE type_, ValueType val_):
	m_isVariable(false), m_type(type_), m_id(nullptr), m_val(std::make_unique<DLVariant>(val_))
{
}
}

#endif
