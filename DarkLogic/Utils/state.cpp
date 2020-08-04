/*===--- state.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file will state class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "state.h"
#include "Operator/alloperator.h"
#include "Variable/allvariable.h"
#include <cassert>

using namespace N_DarkLogic;

unsigned int priorityFromName(Name name)
{
	switch (name)
	{
    case NONE:
    {
        throw std::runtime_error("Unknown operator");
    }
    case AND:
    {
        return AndFun<ASubTheorem>::priority();
    }
    case EQUIVALENT:
    {
        return EqFun<ASubTheorem>::priority();
    }
    case IMPLICATION:
    {
        return ImplFun<ASubTheorem>::priority();
    }
    case NOT:
    {
        return NotFun<ASubTheorem>::priority();
    }
    case OR:
    {
        return OrFun<ASubTheorem>::priority();
    }
    case HYP:
    {
		return HypFun<ASubTheorem>::priority();
    }
    case SETEQUAL:
    {
        return SetEqualFun<SubArithmeticTheorem<NaturalInteger>>::priority();
    }
    case EQUAL:
    {
        return EqualFun<ASubArithmeticTheorem<size_t>, ASubArithmeticTheorem<size_t>>::priority();
    }
    case LET:
    {
        return LetFun<SubArithmeticTheorem<NaturalInteger>, ASubTheorem>::priority();
    }
    case BELONGSTO:
    {
        return BelongsToFun<ASubArithmeticRule<size_t>, ASubArithmeticRule<NaturalIntegerSet>>::priority();
    }
    case PLUS:
    {
        return PlusFun<ASubArithmeticTheorem<size_t>, ASubArithmeticTheorem<size_t>>::priority();
    }
	}
}

Associativity associativityFromName(Name name)
{
    switch (name)
    {
    case NONE:
    {
        throw std::runtime_error("Unknown operator");
    }
    case AND:
    {
        return AndFun<ASubTheorem>::associativity();
    }
    case EQUIVALENT:
    {
        return EqFun<ASubTheorem>::associativity();
    }
    case IMPLICATION:
    {
        return ImplFun<ASubTheorem>::associativity();
    }
    case NOT:
    {
        return NotFun<ASubTheorem>::associativity();
    }
    case OR:
    {
        return OrFun<ASubTheorem>::associativity();
    }
    case HYP:
    {
        return HypFun<ASubTheorem>::associativity();
    }
    case SETEQUAL:
    {
        return SetEqualFun<SubArithmeticTheorem<NaturalInteger>>::associativity();
    }
    case EQUAL:
    {
        return EqualFun<ASubArithmeticTheorem<size_t>, ASubArithmeticTheorem<size_t>>::associativity();
    }
    case LET:
    {
        return LetFun<SubArithmeticTheorem<NaturalInteger>, ASubTheorem>::associativity();
    }
    case BELONGSTO:
    {
        return BelongsToFun<ASubArithmeticRule<size_t>, ASubArithmeticRule<NaturalIntegerSet>>::associativity();
    }
    case PLUS:
    {
        return PlusFun<ASubArithmeticTheorem<size_t>, ASubArithmeticTheorem<size_t>>::associativity();
    }
    }
}

template<typename T>
std::vector<T> mergeVectors(const std::vector<T>& head, const std::vector<T>& queue)
{
	std::vector<T> ret = head;
    for (const auto& queueElmt : queue)
    {
        ret.push_back(queueElmt);
    }
	return ret;
}

template<typename T>
std::vector<T> mergeVectors(const std::vector<T>& left, T middle, const std::vector<T>& right)
{
	std::vector<T> ret = left;
	ret.push_back(middle);
	ret.insert(ret.end(), right.begin(), right.end());
	return ret;
}

std::vector<Name> mergeOperatorVectors(Name headOpe, const std::vector<State>& subStates)
{
	std::vector<Name> ret;
	ret.push_back(headOpe);
	for (const auto& subState : subStates)
	{
		ret.insert(ret.end(), subState.operators().begin(), subState.operators().end());
	}
	return ret;
}

std::vector<State::Term> mergeTermVectors(const std::vector<State>& subStates)
{
	std::vector<State::Term> ret;
	for (const auto& subState : subStates)
	{
        for (const auto& term : subState.terms())
        {
            ret.push_back(term);
        }
	}
	return ret;
}

std::vector<State::OrderedName> fusionOpeName(const std::vector<State::OrderedName>& opeList1,
    const std::vector<State::OrderedName>& opeList2);

std::vector<State::OrderedName> triFusion(const std::vector<State::OrderedName>& opeList)
{
    if (opeList.size() <= 1)
    {
        return opeList;
    }
    else
    {
        auto mid = opeList.begin() + opeList.size() / 2;
        std::vector<State::OrderedName> opeList1(opeList.begin(), mid);
        std::vector<State::OrderedName> opeList2(mid, opeList.end());
        return fusionOpeName(triFusion(opeList1), triFusion(opeList2));
    }
}
/**
 * @brief N_LogicGame::fusion
 * @param opeList1 = ope1::q1
 * @param opeList2 = ope2::q2
 * @return
 */
std::vector<State::OrderedName> fusionOpeName(const std::vector<State::OrderedName>& opeList1,
    const std::vector<State::OrderedName>& opeList2)
{
    if (opeList1.size() == 0)
    {
        return opeList2;
    }
    else if (opeList2.size() == 0)
    {
        return opeList1;
    }
    else
    {
        //if ope1 is in less parenthesis than ope2, then return ope1::fusion(q1,opeList2)
        if (opeList1[0].nbPar() < opeList2[0].nbPar())
        {
            std::vector<State::OrderedName> t1(opeList1.begin(), opeList1.begin() + 1);
            std::vector<State::OrderedName> queueOpeList1(opeList1.begin() + 1, opeList1.end());
            std::vector<State::OrderedName> t2 = fusionOpeName(queueOpeList1, opeList2);
            auto ret = t1;
            ret.insert(ret.end(), t2.begin(), t2.end());
            return ret;
        }
        //if ope2 is in less parenthesis than ope1, then return ope2::fusion(opeList1,q2)
        else if (opeList1[0].nbPar() > opeList2[0].nbPar())
        {
            std::vector<State::OrderedName> t1(opeList2.begin(), opeList2.begin() + 1);
            std::vector<State::OrderedName> queueOpeList2(opeList2.begin() + 1, opeList2.end());
            std::vector<State::OrderedName> t2 = fusionOpeName(opeList1, queueOpeList2);
            auto ret = t1;
            ret.insert(ret.end(), t2.begin(), t2.end());
            return ret;
        }
        //if ope1 and ope2 are in the same parenthesis
        else
        {
            //if ope1 is in less hypothesis operator than ope2, then return ope1::fusion(q1,opeList2)
            if (opeList1[0].nbHyps() < opeList2[0].nbHyps())
            {
                std::vector<State::OrderedName> t1(opeList1.begin(), opeList1.begin() + 1);
                std::vector<State::OrderedName> queueOpeList1(opeList1.begin() + 1, opeList1.end());
                std::vector<State::OrderedName> t2 = fusionOpeName(queueOpeList1, opeList2);
                auto ret = t1;
                ret.insert(ret.end(), t2.begin(), t2.end());
                return ret;
            }
            //if ope2 is in less hypothesis operator than ope1, then return ope2::fusion(opeList1,q2)
            else if (opeList1[0].nbHyps() > opeList2[0].nbHyps())
            {
                std::vector<State::OrderedName> t1(opeList2.begin(), opeList2.begin() + 1);
                std::vector<State::OrderedName> queueOpeList2(opeList2.begin() + 1, opeList2.end());
                std::vector<State::OrderedName> t2 = fusionOpeName(opeList1, queueOpeList2);
                auto ret = t1;
                ret.insert(ret.end(), t2.begin(), t2.end());
                return ret;
            }
            //if ope1 and ope2 are in the same number of hypothesis operator
            else
            {
                //if ope1 comes before ope2 in the arguments of the current function, then return ope1::fusion(q1,opeList2)
                if (opeList1[0].argIdx() < opeList2[0].argIdx())
                {
                    std::vector<State::OrderedName> t1(opeList1.begin(), opeList1.begin() + 1);
                    std::vector<State::OrderedName> queueOpeList1(opeList1.begin() + 1, opeList1.end());
                    std::vector<State::OrderedName> t2 = fusionOpeName(queueOpeList1, opeList2);
                    auto ret = t1;
                    ret.insert(ret.end(), t2.begin(), t2.end());
                    return ret;
                }
                //if ope2 comes before ope1 in the arguments of the current function, then return ope2::fusion(opeList1,q2)
                else if (opeList1[0].argIdx() > opeList2[0].argIdx())
                {
                    std::vector<State::OrderedName> t1(opeList2.begin(), opeList2.begin() + 1);
                    std::vector<State::OrderedName> queueOpeList2(opeList2.begin() + 1, opeList2.end());
                    std::vector<State::OrderedName> t2 = fusionOpeName(opeList1, queueOpeList2);
                    auto ret = t1;
                    ret.insert(ret.end(), t2.begin(), t2.end());
                    return ret;
                }
                //if ope1 and ope2 are in the same argument of the current function
                else
                {
                    //IOperator* ope1=opeList1[0].ope;
                    //IOperator* ope2=opeList2[0].ope;
                    if (opeList1[0].priority() > opeList2[0].priority())
                    {
                        std::vector<State::OrderedName> t1(opeList1.begin(), opeList1.begin() + 1);
                        std::vector<State::OrderedName> queueOpeList1(opeList1.begin() + 1, opeList1.end());
                        std::vector<State::OrderedName> t2 = fusionOpeName(queueOpeList1, opeList2);
                        auto ret = t1;
                        ret.insert(ret.end(), t2.begin(), t2.end());
                        return ret;
                    }
                    else if (opeList1[0].priority() < opeList2[0].priority())
                    {
                        std::vector<State::OrderedName> t1(opeList2.begin(), opeList2.begin() + 1);
                        std::vector<State::OrderedName> queueOpeList2(opeList2.begin() + 1, opeList2.end());
                        std::vector<State::OrderedName> t2 = fusionOpeName(opeList1, queueOpeList2);
                        auto ret = t1;
                        ret.insert(ret.end(), t2.begin(), t2.end());
                        return ret;
                    }
                    else
                    {
                        switch (opeList1[0].associativity())
                        {
                        case Associativity::LEFT:
                        {
                            std::vector<State::OrderedName> t1(opeList1.begin(), opeList1.begin() + 1);
                            std::vector<State::OrderedName> queueOpeList1(opeList1.begin() + 1, opeList1.end());
                            std::vector<State::OrderedName> t2 = fusionOpeName(queueOpeList1, opeList2);
                            auto ret = t1;
                            ret.insert(ret.end(), t2.begin(), t2.end());
                            return ret;
                        }
                        case Associativity::RIGHT:
                        {
                            std::vector<State::OrderedName> t1(opeList2.begin(), opeList2.begin() + 1);
                            std::vector<State::OrderedName> queueOpeList2(opeList2.begin() + 1, opeList2.end());
                            std::vector<State::OrderedName> t2 = fusionOpeName(opeList1, queueOpeList2);
                            auto ret = t1;
                            ret.insert(ret.end(), t2.begin(), t2.end());
                            return ret;
                        }
                        }
                    }
                }
            }
        }
    }
    return std::vector<State::OrderedName>();
}

std::vector<State::OrderedName> orderOperators(Name headOpe, const State& subState)
{
	std::vector<State::OrderedName> ret;
	const auto& orderedOpeSub = subState.priorityOpe();
	ret.push_back(State::OrderedName(headOpe, 0, 0, 0));
	if (orderedOpeSub.size())
	{
		if (priorityFromName(headOpe) < priorityFromName(orderedOpeSub[0].name()))
		{
			for (const auto& opeSub : orderedOpeSub)
			{
				ret.push_back(State::OrderedName(opeSub.name(), opeSub.nbPar() + 1, opeSub.nbHyps(), opeSub.argIdx()));
			}
		}
		else
		{
			ret.insert(ret.end(), orderedOpeSub.begin(), orderedOpeSub.end());
		}
	}

	return ret;
}

std::vector<State::OrderedName> orderOperators(Name headOpe, const State& leftState, const State& rightState)
{
	std::vector<State::OrderedName> ret;
	const auto& leftOrderedOpe = leftState.priorityOpe();
	const auto& rightOrderedOpe = rightState.priorityOpe();
	std::vector<State::OrderedName> orderedOpe;
	ret.push_back(State::OrderedName(headOpe, 0, 0, 0));
	if (leftOrderedOpe.size())
	{
		if (priorityFromName(headOpe) < priorityFromName(leftOrderedOpe[0].name()))
		{
			for (const auto& crtOpe : leftOrderedOpe)
			{
				orderedOpe.push_back(State::OrderedName(crtOpe.name(), crtOpe.nbPar() + 1, crtOpe.nbHyps(), crtOpe.argIdx()));
			}
		}
		else
		{
			for (const auto& crtOpe : leftOrderedOpe)
			{
				orderedOpe.push_back(crtOpe);
			}
		}

		if (rightOrderedOpe.size())
		{
			if (priorityFromName(headOpe) < priorityFromName(rightOrderedOpe[0].name()))
			{
				for (const auto& crtOpe : rightOrderedOpe)
				{
					orderedOpe.push_back(State::OrderedName(crtOpe.name(), crtOpe.nbPar() + 1, crtOpe.nbHyps(), crtOpe.argIdx()));
				}
			}
			else
			{
				for (const auto& crtOpe : rightOrderedOpe)
				{
					orderedOpe.push_back(crtOpe);
				}
			}
		}
	}
	else
	{
		if (rightOrderedOpe.size())
		{
			if (priorityFromName(headOpe) < priorityFromName(rightOrderedOpe[0].name()))
			{
				for (const auto& crtOpe : rightOrderedOpe)
				{
					orderedOpe.push_back(State::OrderedName(crtOpe.name(), crtOpe.nbPar() + 1, crtOpe.nbHyps(), crtOpe.argIdx()));
				}
			}
			else
			{
				for (const auto& crtOpe : rightOrderedOpe)
				{
					orderedOpe.push_back(crtOpe);
				}
			}
		}
	}

    orderedOpe = triFusion(orderedOpe);
    ret.insert(ret.end(), orderedOpe.begin(), orderedOpe.end());
	return ret;
}

std::vector<State::OrderedName> orderOperators(Name headOpe, const std::vector<State>& subStates)
{
    std::vector<State::OrderedName> ret;
    std::vector<State::OrderedName> orderedOpe;
    ret.push_back(State::OrderedName(headOpe, 0, 0, 0));
    size_t argIdx = 0;
    for (const auto& subState : subStates)
    {
        const auto& orderedOpeSub = subState.priorityOpe();
        if (orderedOpeSub.size())
        {
            if (priorityFromName(headOpe) < priorityFromName(orderedOpeSub[0].name()))
            {
                for (const auto& crtOpe : orderedOpeSub)
                {
                    if (crtOpe.nbHyps() == 0)
                    {
                        orderedOpe.push_back(State::OrderedName(crtOpe.name(), crtOpe.nbPar() + 1, 1, argIdx));
                    }
                    else
                    {
                        orderedOpe.push_back(State::OrderedName(crtOpe.name(), crtOpe.nbPar() + 1, crtOpe.nbHyps() + 1, crtOpe.argIdx()));
                    }                    
                }
            }
            else
            {
                for (const auto& crtOpe : orderedOpeSub)
                {
                    if (crtOpe.nbHyps() == 0)
                    {
                        orderedOpe.push_back(State::OrderedName(crtOpe.name(), crtOpe.nbPar(), 1, argIdx));
                    }
                    else
                    {
                        orderedOpe.push_back(State::OrderedName(crtOpe.name(), crtOpe.nbPar(), crtOpe.nbHyps() + 1, crtOpe.argIdx()));
                    }
                }
            }
        }
        argIdx++;
    }
    orderedOpe = triFusion(orderedOpe);
    ret.insert(ret.end(), orderedOpe.begin(), orderedOpe.end());
    return ret;
}

State::State(Name headOpe, const State& subState):
	m_priorityOpe(orderOperators(headOpe, subState)), m_operators(mergeVectors({ headOpe }, subState.operators())),
	m_terms(subState.terms())
{
    assert(m_priorityOpe.size() == m_operators.size());
}

State::State(Name headOpe, const State& leftState, const State& rightState) :
	m_priorityOpe(orderOperators(headOpe, leftState, rightState)), 
	m_operators(mergeVectors(leftState.operators(), headOpe, rightState.operators())),
	m_terms(mergeVectors(leftState.terms(), rightState.terms()))
{
    assert(m_priorityOpe.size() == m_operators.size());
}

State::State(Name headOpe, const std::vector<State>& subStates):
	m_priorityOpe(orderOperators(headOpe, subStates)),
	m_operators(mergeOperatorVectors(headOpe,subStates)),
	m_terms(mergeTermVectors(subStates))
{
    assert(m_priorityOpe.size() == m_operators.size());
}

State::State(const IDVar& idVar, VALUE_TYPE varType):
	m_terms({ Term(idVar, varType) })
{
}

const std::vector<State::OrderedName>& State::priorityOpe() const
{
	return m_priorityOpe;
}

const std::vector<Name>& State::operators() const
{
	return m_operators;
}

const std::vector<State::Term>& State::terms() const
{
	return m_terms;
}

N_DarkLogic::State::Term::Term(IDVar id_, VALUE_TYPE type_) :
    m_isVariable(true), m_type(type_), m_id(std::make_unique<IDVar>(id_)), m_val(nullptr)
{
}

N_DarkLogic::State::Term::Term(const Term& term):
	m_isVariable(term.m_isVariable), m_type(term.m_type), m_id(nullptr), 
    m_val(nullptr)
{
    if (m_isVariable)
    {
        m_id = std::make_unique<IDVar>(term.id());
    }
    else
    {
        m_val = std::make_unique<DLVariant>(term.val());
    }
}

bool N_DarkLogic::State::Term::isVariable() const
{
	return m_isVariable;
}

VALUE_TYPE N_DarkLogic::State::Term::type() const
{
	return m_type;
}

const IDVar& N_DarkLogic::State::Term::id() const
{
    return *m_id;
}

const DLVariant& N_DarkLogic::State::Term::val() const
{
	return *m_val;
}

N_DarkLogic::State::OrderedName::OrderedName(Name name_, const unsigned int& nbPar_, const unsigned int& nbHyps_, 
    const unsigned int& argIdx_):
	m_name(name_), m_priority(priorityFromName(name_)), m_associativity(associativityFromName(name_)), m_nbPar(nbPar_), m_nbHyps(nbHyps_), 
    m_argIdx(argIdx_)
{
}

Name N_DarkLogic::State::OrderedName::name() const
{
	return m_name;
}

unsigned int N_DarkLogic::State::OrderedName::priority() const
{
    return m_priority;
}

Associativity N_DarkLogic::State::OrderedName::associativity() const
{
    return m_associativity;
}

unsigned int N_DarkLogic::State::OrderedName::nbHyps() const
{
    return m_nbHyps;
}

unsigned int N_DarkLogic::State::OrderedName::nbPar() const
{
	return m_nbPar;
}

unsigned int N_DarkLogic::State::OrderedName::argIdx() const
{
	return m_argIdx;
}
