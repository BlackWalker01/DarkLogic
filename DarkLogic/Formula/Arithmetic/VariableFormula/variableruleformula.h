/*===--- variableruleformula.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubArithmeticRule in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_VARIABLERULEFORMULA_H
#define DARK_LOGIC_VARIABLERULEFORMULA_H
#include "Formula/Arithmetic/subarithmeticrule.h"
#include "Formula/dbvarprop.h"
#include "logic.h"

namespace N_DarkLogic
{
template<SubRuleFormula VarType>
class SubArithmeticRule:
        public ASubArithmeticRule<typename VarType::ValueType>
{
public:
    typedef VarType SubOperatorType;
    typedef typename SubOperatorType::ValueType ValueType;
    typedef typename
    ASubArithmeticRule<typename VarType::ValueType>::ATheoremType ATheoremType;

    SubArithmeticRule(const std::shared_ptr<VarType>& var);

    size_t arity() const override final;
    ValueType evaluate() const override final;
    constexpr ArithType type() const override final;

    //internal methods
    bool identifyPriv(const ptr<ATheoremType>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const override final;
    ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const override final;

    const VarType& getSon() const;

    void operator=(const ValueType& val) const;

    bool isEqual(const ASubArithmeticRule<ValueType>& prop) const override final;
    bool isEqual(const ASubArithmeticTheorem<ValueType>& prop) const override final;
    bool operator==(const SubArithmeticRule& prop) const;
    bool operator==(const SubArithmeticTheorem<SubOperatorType>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;

    ~SubArithmeticRule() override = default;

    const std::shared_ptr<SubOperatorType> m_son;
    const DbVar m_extVars;
};

template<ArithmeticType ValueType> 
struct ToTheoremStruct<SubArithmeticRule<ValueType>> { using Type = SubArithmeticTheorem<ValueType>; };

template<SubRuleFormula VarType>
SubArithmeticRule<VarType>::SubArithmeticRule(const std::shared_ptr<VarType> &var):
    m_son(var), m_extVars(var)
{
    
}

template<SubRuleFormula VarType>
constexpr ArithType SubArithmeticRule<VarType>::type() const
{
    return ToArithType<ValueType>::convert();
}

template<SubRuleFormula VarType>
size_t SubArithmeticRule<VarType>::arity() const
{
    return 0;
}

template<SubRuleFormula VarType>
typename SubArithmeticRule<VarType>::ValueType SubArithmeticRule<VarType>::evaluate() const
{
    return m_son->evaluate();
}


template<SubRuleFormula VarType>
bool SubArithmeticRule<VarType>::
identifyPriv(const ptr<typename SubArithmeticRule<VarType>::ATheoremType> &prop, DbVarProp &dbVarProp) const
{
    if(!dbVarProp.contains(m_son->id()))
    {
        dbVarProp[m_son->id()]=prop;
        return true;
    }
    else
    {
        auto exProp=dbVarProp[m_son->id()];
        auto exPropCast=std::dynamic_pointer_cast<const ASubArithmeticRule<ValueType>>(exProp);
        if(exPropCast)
        {
            return *(prop)==*exPropCast;
        }
        else
        {
            return false;
        }

    }
}

template<SubRuleFormula VarType>
ptr<typename SubArithmeticRule<VarType>::ATheoremType>
SubArithmeticRule<VarType>::
applyPriv(DbVarProp &dbVarProp) const
{
    return std::static_pointer_cast<const ATheoremType>(dbVarProp[m_son->id()]);
}


template<SubRuleFormula VarType>
ptr<typename SubArithmeticRule<VarType>::ATheoremType>
SubArithmeticRule<VarType>::applyFirstPriv(DbVarProp &dbVarProp) const
{
    return std::static_pointer_cast<const ATheoremType>(dbVarProp[m_son->id()]);
}

template<SubRuleFormula VarType>
ptr<typename SubArithmeticRule<VarType>::ATheoremType>
SubArithmeticRule<VarType>::
applyPriv(DbVarProp& dbVarProp, const size_t&) const
{
    return std::static_pointer_cast<const ATheoremType>(dbVarProp[m_son->id()]);
}


template<SubRuleFormula VarType>
ptr<typename SubArithmeticRule<VarType>::ATheoremType>
SubArithmeticRule<VarType>::applyFirstPriv(DbVarProp& dbVarProp, const size_t&) const
{
    return std::static_pointer_cast<const ATheoremType>(dbVarProp[m_son->id()]);
}


template<SubRuleFormula VarType>
const VarType &SubArithmeticRule<VarType>::getSon() const
{
    return *(m_son.get());
}

template<SubRuleFormula VarType>
void SubArithmeticRule<VarType>::operator=(const ValueType &val) const
{
    *m_son=val;
}

template<SubRuleFormula VarType>
bool SubArithmeticRule< VarType>::
isEqual(const ASubArithmeticTheorem<ValueType>& prop) const
{
    auto propCast=dynamic_cast<const SubArithmeticTheorem<VarType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<SubRuleFormula VarType>
bool SubArithmeticRule< VarType>::
isEqual(const ASubArithmeticRule<ValueType>& prop) const
{
    auto propCast=dynamic_cast<const SubArithmeticRule*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<SubRuleFormula VarType>
bool SubArithmeticRule<VarType>::
operator==(const SubArithmeticRule<VarType> &prop) const
{
    return *m_son==(prop.getSon());
}

template<SubRuleFormula VarType>
bool SubArithmeticRule<VarType>::operator==(const SubArithmeticTheorem<VarType> &prop) const
{
    return *m_son==(prop.getSon());
}

template<SubRuleFormula VarType>
std::string SubArithmeticRule<VarType>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<SubRuleFormula VarType>
const DbVar* SubArithmeticRule<VarType>::getExtVars() const
{
    return &m_extVars;
}
}

#endif // DARK_LOGIC_VARIABLERULEFORMULA_H
