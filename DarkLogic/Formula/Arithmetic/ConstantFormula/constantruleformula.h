/*===--- constantruleformula.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines ConstSubArithmeticRule in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_CONSTANTRULEFORMULA_H
#define DARK_LOGIC_CONSTANTRULEFORMULA_H
#include "Formula/Arithmetic/subarithmeticrule.h"
#include "Formula/dbvarprop.h"
#include "logic.h"

namespace N_DarkLogic
{

template<ArithConstantType VarType>
class ConstSubArithmeticTheorem;

template<ArithConstantType VarType>
class ConstSubArithmeticRule:
        public ASubArithmeticRule<typename VarType::ValueType>
{
public:
    typedef VarType SubOperatorType;
    typedef typename SubOperatorType::ValueType ValueType;
    typedef typename
    ASubArithmeticRule<typename VarType::ValueType>::ATheoremType ATheoremType;

    ConstSubArithmeticRule(const ValueType& var);

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

    bool isEqual(const ASubArithmeticRule<ValueType>& prop) const override final;
    bool isEqual(const ASubArithmeticTheorem<ValueType>& prop) const override final;
    bool operator==(const ConstSubArithmeticRule& prop) const;
    bool operator==(const ConstSubArithmeticTheorem<SubOperatorType>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;

    ~ConstSubArithmeticRule() override = default;

protected:
    const std::unique_ptr<const SubOperatorType> m_son;
};


template<ArithmeticType ValueType>
struct ToTheoremStruct<ConstSubArithmeticRule<ValueType>> { using Type = ConstSubArithmeticTheorem<ValueType>; };

template<ArithConstantType VarType>
ConstSubArithmeticRule<VarType>::ConstSubArithmeticRule(const ValueType &var):
    m_son(std::make_unique<SubOperatorType>(var))
{
    
}

template<ArithConstantType VarType>
size_t ConstSubArithmeticRule<VarType>::arity() const
{
    return 0;
}

template<ArithConstantType VarType>
typename ConstSubArithmeticRule<VarType>::ValueType ConstSubArithmeticRule<VarType>::evaluate() const
{
    return m_son->evaluate();
}

template<ArithConstantType VarType>
inline constexpr ArithType ConstSubArithmeticRule<VarType>::type() const
{
    return ToArithType<ValueType>::convert();
}


template<ArithConstantType VarType>
bool ConstSubArithmeticRule<VarType>::
identifyPriv(const ptr<typename ConstSubArithmeticRule<VarType>::ATheoremType> &prop, DbVarProp &/*dbVarProp*/) const
{
   return (*this)==*prop;
}

template<ArithConstantType VarType>
ptr<typename ConstSubArithmeticRule<VarType>::ATheoremType>
ConstSubArithmeticRule<VarType>::
applyPriv(DbVarProp &/*dbVarProp*/) const
{
    return Logic::make_theorem_formula<ConstSubArithmeticTheorem<VarType>>(m_son->evaluate());
}


template<ArithConstantType VarType>
ptr<typename ConstSubArithmeticRule<VarType>::ATheoremType>
ConstSubArithmeticRule<VarType>::applyFirstPriv(DbVarProp &/*dbVarProp*/) const
{
    return Logic::make_theorem_formula<ConstSubArithmeticTheorem<VarType>>(m_son->evaluate());
}


template<ArithConstantType VarType>
ptr<typename ConstSubArithmeticRule<VarType>::ATheoremType>
ConstSubArithmeticRule<VarType>::
applyPriv(DbVarProp&/*dbVarProp*/, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<ConstSubArithmeticTheorem<VarType>>(logicIdx, m_son->evaluate());
}


template<ArithConstantType VarType>
ptr<typename ConstSubArithmeticRule<VarType>::ATheoremType>
ConstSubArithmeticRule<VarType>::applyFirstPriv(DbVarProp&/*dbVarProp*/, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<ConstSubArithmeticTheorem<VarType>>(logicIdx, m_son->evaluate());
}


template<ArithConstantType VarType>
const VarType &ConstSubArithmeticRule<VarType>::getSon() const
{
    return *(m_son.get());
}

template<ArithConstantType VarType>
bool ConstSubArithmeticRule< VarType>::
isEqual(const ASubArithmeticTheorem<ValueType>& prop) const
{
    auto propCast=
            dynamic_cast<const ConstSubArithmeticTheorem<VarType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<ArithConstantType VarType>
bool ConstSubArithmeticRule< VarType>::
isEqual(const ASubArithmeticRule<ValueType>& prop) const
{
    auto propCast=dynamic_cast<const ConstSubArithmeticRule*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<ArithConstantType VarType>
bool ConstSubArithmeticRule<VarType>::
operator==(const ConstSubArithmeticRule<VarType> &prop) const
{
    return *m_son==(prop.getSon());
}

template<ArithConstantType VarType>
bool ConstSubArithmeticRule<VarType>::operator==(const ConstSubArithmeticTheorem<VarType> &prop) const
{
    return *m_son==(prop.getSon());
}

template<ArithConstantType VarType>
std::string ConstSubArithmeticRule<VarType>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}
template<ArithConstantType VarType>
inline const DbVar* ConstSubArithmeticRule<VarType>::getExtVars() const
{
    return nullptr;
}
}

#endif // DARK_LOGIC_CONSTANTRULEFORMULA_H
