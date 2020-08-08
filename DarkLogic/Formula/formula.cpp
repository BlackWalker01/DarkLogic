/*===--- formula.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements formula class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/


#include "formula.h"
#include "formula.h"
#include "Operator/ioperator.h"
#include "Variable/term.h"
#include "Set/booleanset.h"
#include "Set/naturalintegerset.h"
#include "Variable/allvariable.h"
#include "Variable/dbvar.h"
#include "Formula/Boolean/asubrule.h"
#include "Formula/Boolean/asubtheorem.h"

using namespace N_DarkLogic;

template<>
VALUE_TYPE AbstractFormula<void>::valueType() const
{
    return VOID_TYPE;
}

std::string N_DarkLogic::FormulaHelper::getVarNameFromId(const IDVar& idVar)
{
    return AVariable::getVarNameFromId(idVar);
}

template<>
VALUE_TYPE AbstractFormula<bool>::valueType() const
{
    return BOOL_TYPE;
}

template<>
VALUE_TYPE AbstractFormula<UInt>::valueType() const
{
    return NATURAL_INT_TYPE;
}

template<>
VALUE_TYPE AbstractFormula<BooleanSet>::valueType() const
{
    return BOOLEANSET_TYPE;
}

template<>
VALUE_TYPE AbstractFormula<NaturalIntegerSet>::valueType() const
{
    return NATURAL_INTEGERSET_TYPE;
}

std::vector<const DbVar*> N_DarkLogic::getDbVarFromRules(const std::vector<ptr<ASubRule> > &formulas)
{
    std::vector<const DbVar*> ret;
    for(size_t k=0;k<formulas.size();k++)
    {
        ret.push_back(formulas[k]->getExtVars());
    }
    return ret;
}

std::vector<const DbVar*> N_DarkLogic::getDbVarFromTheorems(const std::vector<ptr<ASubTheorem> > &formulas)
{
    std::vector<const DbVar*> ret;
    for(size_t k=0;k<formulas.size();k++)
    {
        ret.push_back(formulas[k]->getExtVars());
    }
    return ret;
}
