#ifndef FORMULA_H
#define FORMULA_H
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Variable/dbvar.h"
#include "valuetypeobject.h"
#include "Utils/utils.h"

namespace N_Logic
{

class IOperator;
class ASubRule;
class ASubTheorem;

template<typename ValueType>
class AbstractFormula: public ValueTypeObject
{
public:
    AbstractFormula(const std::string& name_);
    AbstractFormula(const std::string& name_, const DbVar& dbVar);
    AbstractFormula(const std::string& name_, const DbVar& dbVar, const DbVar& dbVar2);
    AbstractFormula(const std::string& name_, const std::vector<DbVar>& dbVars);
    AbstractFormula(const AbstractFormula& formula) = delete;

    std::string name() const;
    virtual std::string toString(unsigned short priorityParent=1000) const=0;
    void print() const;

    virtual ValueType evaluate() const =0;
    VALUE_TYPE valueType() const override final;

    const DbVar& getExtVars() const;
    std::vector<std::string> nameVars() const;

    static std::vector<DbVar> getDbVarFromFormulas(const std::vector<ptr<AbstractFormula> >& formulas);


protected:
    //Attributes
    const std::string m_name;
    const DbVar m_extVars;
};

struct OperatorOrdering
{
    OperatorOrdering();
    OperatorOrdering(const ptr<IOperator>& ope_, const size_t& nbPar_, const size_t& argIndex_);
    OperatorOrdering(const OperatorOrdering& opeOrdering);

    std::shared_ptr<const IOperator> ope; //operator to order
    size_t nbPar; //number of unclosed parenthesis before the operator
    size_t argIndex; //index of the operator in the current parenthesis
};

struct ParenthesisParam
{
    ParenthesisParam();
    ParenthesisParam(const size_t& nbPar_, const size_t& nbArgs_, const size_t& indexInOpeList_);
    ParenthesisParam(const ParenthesisParam& parenthesisParam);

    size_t nbPar;
    size_t nbArgs;
    size_t indexInOpeList;
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

template<typename ValueType>
AbstractFormula<ValueType>::AbstractFormula(const std::string &name_, const DbVar &dbVar):
    m_name(name_), m_extVars(dbVar)
{

}

template<typename ValueType>
AbstractFormula<ValueType>::AbstractFormula(const std::string &name_, const DbVar &dbVar, const DbVar &dbVar2):
    m_name(name_), m_extVars(dbVar,dbVar2)
{

}

template<typename ValueType>
AbstractFormula<ValueType>::AbstractFormula(const std::string &name_, const std::vector<DbVar> &dbVars):
    m_name(name_), m_extVars(dbVars)
{

}

template<typename ValueType>
std::vector<DbVar> AbstractFormula<ValueType>::getDbVarFromFormulas(const std::vector<ptr<AbstractFormula> > &formulas)
{
    std::vector<DbVar> ret;
    for(size_t k=0;k<formulas.size();k++)
    {
        ret.push_back(formulas[k]->getExtVars());
    }
    return ret;
}

std::vector<DbVar> getDbVarFromRules(const std::vector<ptr<ASubRule>>& formulas);
std::vector<DbVar> getDbVarFromTheorems(const std::vector<ptr<ASubTheorem>>& formulas);

#include "formula.hxx"

}

#endif // FORMULA_H
