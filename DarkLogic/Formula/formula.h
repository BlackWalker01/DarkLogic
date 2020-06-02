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
    void print() const;

    virtual ValueType evaluate() const =0;
    virtual const State& getState() const = 0;
    VALUE_TYPE valueType() const override final;

    virtual const DbVar* getExtVars() const = 0;
};


struct ParenthesisParam
{
    ParenthesisParam();
    ParenthesisParam(const size_t& nbPar_, const size_t& indexInOpeList_);
    ParenthesisParam(const ParenthesisParam& parenthesisParam);

    size_t nbPar;
    size_t indexInOpeList;
};

struct OperatorOrdering
{
    OperatorOrdering();
    //OperatorOrdering(const ptr<IOperator>& ope_, const size_t& nbPar_);
    OperatorOrdering(const ptr<IOperator>& ope_, const size_t& nbPar_, const size_t& argIndex_);
    OperatorOrdering(const OperatorOrdering& opeOrdering);

    std::shared_ptr<const IOperator> ope; //operator to order
    size_t nbPar; //number of unclosed parenthesis before the operator
    size_t argIndex; //index in variadic operator over this operator
    size_t nbArgs;
};

struct VariableContainer
{
    VariableContainer(const std::shared_ptr<AbstractTerm> &term);

    void init();

    ~VariableContainer() = default;

    //attributes
    VALUE_TYPE type;
    std::shared_ptr<AbstractTerm> var;
};

class DbVarContainer
{
public:
    DbVarContainer() = default;

    void insert(const std::shared_ptr<VariableContainer> &var);

    std::shared_ptr<VariableContainer> &find(const std::string &name);
    bool contains(const std::string& name) const;

    ~DbVarContainer() = default;

private:
    std::unordered_map<std::string,std::shared_ptr<VariableContainer>> db;
};

std::vector<OperatorOrdering> orderOperator(std::vector<OperatorOrdering>& opeList);
std::vector<OperatorOrdering> fusion(const std::vector<OperatorOrdering >& opeList1,
                                                         const std::vector<OperatorOrdering >& opeList2);

std::vector<const DbVar*> getDbVarFromRules(const std::vector<ptr<ASubRule>>& formulas);
std::vector<const DbVar*> getDbVarFromTheorems(const std::vector<ptr<ASubTheorem>>& formulas);

#include "formula.hxx"

}

#endif // DARK_LOGIC_FORMULA_H
