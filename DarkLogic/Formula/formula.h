/*===--- formula.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines AbstractFormula in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_FORMULA_H
#define DARK_LOGIC_FORMULA_H
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Variable/dbvar.h"
#include "valuetypeobject.h"
#include "Utils/utils.h"
#include "Utils/state.h"

namespace N_DarkLogic
{

class IOperator;
class ASubRule;
class ASubTheorem;
class AbstractTerm;

template<Type ValueType_>
class AbstractFormula: public ValueTypeObject
{
public:
    using ValueType = ValueType_;

    AbstractFormula() = default;
    AbstractFormula(const AbstractFormula& formula) = delete;
    
    virtual std::string toString(unsigned short priorityParent=1000) const=0;
    std::string toNormalizedString(unsigned short priorityParent = 1000) const;
    void print() const;

    virtual ValueType evaluate() const =0;
    virtual const State& getState() const = 0;
    VALUE_TYPE valueType() const override final;

    virtual const DbVar* getExtVars() const = 0;
};

namespace FormulaHelper
{
    std::string getVarNameFromId(const IDVar& idVar);
}

std::vector<const DbVar*> getDbVarFromRules(const std::vector<ptr<ASubRule>>& formulas);
std::vector<const DbVar*> getDbVarFromTheorems(const std::vector<ptr<ASubTheorem>>& formulas);

#include "formula.hxx"

}

#endif // DARK_LOGIC_FORMULA_H
