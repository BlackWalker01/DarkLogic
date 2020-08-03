/*===--- asubrule.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements asubrule class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "asubrule.h"
#include "Operator/alloperator.h"
#include "Variable/dbvar.h"
#include "Variable/allvariable.h"
#include "Formula/Arithmetic/VariableFormula/variableruleformula.h"

using namespace N_DarkLogic;

std::string N_DarkLogic::ASubRule::name() const
{
    throw std::runtime_error("SubRule cannot call name method");
}

ptr<IISubTheoremFormula> ASubRule::applyAnnexe(const Action::Id &, const ptr<IISubTheoremFormula> &, DbVarProp&, std::vector<Arity> &) const
{
    throw std::runtime_error("SubRule cannot call applyAnnexe method");
}

ptr<IISubTheoremFormula> ASubRule::applyAnnexe(const Action::Id&, const ptr<IISubTheoremFormula>&, DbVarProp&, std::vector<Arity>&,
    const size_t&) const
{
    throw std::runtime_error("SubRule cannot call applyAnnexe method");
}

bool ASubRule::isSymetric() const
{
	return false;
}

void N_DarkLogic::ASubRule::clearAllIdentifications() const
{
    throw std::runtime_error("SubRule cannot call clearAllIdentifications method");
}

bool N_DarkLogic::ASubRule::testEvaluate() const
{
	return evaluate();
}


ptr<IOperator> N_DarkLogic::createRuleOperator(const Name name, const Arity &arity)
{
    switch (name)
    {
        case NONE:
        {
            throw std::runtime_error("Unknown operator");
        }
        case AND:
        {
            return std::make_shared<const And<ASubRule>>();
        }
        case EQUIVALENT:
        {
            return std::make_shared<const Equivalent<ASubRule>>();
        }
        case IMPLICATION:
        {
            return std::make_shared<const Implication<ASubRule>>();
        }
        case NOT:
        {
            return std::make_shared<const Not<ASubRule>>();
        }
        case OR:
        {
            return std::make_shared<const Or<ASubRule>>();
        }
        case HYP:
        {
            std::vector<ptr<ASubRule>> props;
            for(size_t k=0;k<arity;k++)
            {
                props.push_back(nullptr);
            }
            return std::make_shared<const Hyp<ASubRule>>(props);
        }
        case EQUAL:
        {
            return std::make_shared<const Equal<ASubArithmeticRule<size_t>,ASubArithmeticRule<size_t>>>();
        }
        case SETEQUAL:
        {
            return std::make_shared<const SetEqual<SubArithmeticRule<NaturalInteger>>>();
        }
        case LET:
        {
            return std::make_shared<const Let<SubArithmeticRule<NaturalInteger>,ASubRule>>();
        }
        case BELONGSTO:
        {
            return std::make_shared<const BelongsTo<ASubArithmeticRule<size_t>,ASubArithmeticRule<NaturalIntegerSet>>>();
        }
        case PLUS:
        {
            return std::make_shared<const Plus<ASubArithmeticRule<size_t>,ASubArithmeticRule<size_t>>>();
        }
    }

    return nullptr;
}

void N_DarkLogic::addRuleOperator(const Name name, std::vector<OperatorOrdering> &opeList,
                                  const std::vector<OperatorOrdering> &hyps, const std::vector<HypParams>& hypStack,
                                const size_t& numPar)
{
    ptr<IOperator> ope=nullptr;
    if((ope=createRuleOperator(name)))
    {
        opeList.push_back(OperatorOrdering(ope, numPar, hypStack));
    }
    else
    {
        throw std::runtime_error("Unknown operator");
    }
}
