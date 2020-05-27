/*===--- variabletheoremformula.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubArithmeticTheorem in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_VARIABLETHEOREMFORMULA_H
#define DARK_LOGIC_VARIABLETHEOREMFORMULA_H
#include "Formula/Arithmetic/subarithmetictheorem.h"

namespace N_DarkLogic
{
template<SubTheoremFormula VarType>
class SubArithmeticTheorem:
        public ASubArithmeticTheorem<typename VarType::ValueType>
{
public:
    typedef VarType SubOperatorType;
    using SubPropertyType = SubOperatorType;
    typedef typename SubOperatorType::ValueType ValueType;

    SubArithmeticTheorem(const std::shared_ptr<VarType>& var);

    size_t arity() const override final;
    ValueType evaluate() const override final;
    const State& getState() const override final;
    constexpr ArithType type() const override final;

    const SubOperatorType& getSon() const;
    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;
    const std::vector<std::vector<Arity>>& computeAllPaths() override final;
    const std::vector<std::vector<Arity>>& computeImplPaths() override final;

    void operator=(const ValueType& val) const;

    bool isEqual(const ASubArithmeticRule<ValueType>& prop) const override final;
    bool isEqual(const ASubArithmeticTheorem<ValueType>& prop) const override final;
    bool operator==(const SubArithmeticTheorem& prop) const;
    bool operator==(const SubArithmeticRule<SubOperatorType>& prop) const;

    ~SubArithmeticTheorem() override = default;

    const std::shared_ptr<SubOperatorType> m_son;
    const DbVar m_extVars;
    const State m_state;
};

template<ArithmeticType ValueType> struct ToRuleStruct<SubArithmeticTheorem<ValueType>> { using Type = SubArithmeticRule<ValueType>; };

template<SubTheoremFormula VarType>
SubArithmeticTheorem<VarType>::SubArithmeticTheorem(const std::shared_ptr<VarType> &var):
    m_son(var), m_extVars(var), m_state(m_son->id(), m_son->valueType())
{
    computeAllPaths();
    computeImplPaths();
}

template<SubTheoremFormula VarType>
inline const State& SubArithmeticTheorem<VarType>::getState() const
{
    return m_state;
}

template<SubTheoremFormula VarType>
constexpr ArithType SubArithmeticTheorem<VarType>::type() const
{
    return ToArithType<ValueType>::convert();
}

template<SubTheoremFormula VarType>
size_t SubArithmeticTheorem<VarType>::arity() const
{
    return 0;
}

template<SubTheoremFormula VarType>
typename SubArithmeticTheorem<VarType>::ValueType SubArithmeticTheorem<VarType>::evaluate() const
{
    return m_son->evaluate();
}

template<SubTheoremFormula VarType>
const typename SubArithmeticTheorem<VarType>::SubOperatorType &SubArithmeticTheorem<VarType>::getSon() const
{
    return *(m_son.get());
}

template<SubTheoremFormula VarType>
std::string SubArithmeticTheorem<VarType>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<SubTheoremFormula VarType>
const DbVar* SubArithmeticTheorem<VarType>::getExtVars() const
{
    return &m_extVars;
}

template<SubTheoremFormula VarType>
const std::vector<std::vector<Arity> > &SubArithmeticTheorem<VarType>::computeAllPaths()
{
    if(!this->m_allPaths.size())
    {
        this->m_allPaths.push_back({});
    }
    return this->m_allPaths;
}


template<SubTheoremFormula VarType>
const std::vector<std::vector<Arity> >& SubArithmeticTheorem<VarType>::computeImplPaths()
{
    if (!this->m_implPaths.size())
    {
        this->m_implPaths.push_back({});
    }
    return this->m_implPaths;
}


template<SubTheoremFormula VarType>
void SubArithmeticTheorem<VarType>::operator=(const ValueType &val) const
{
    *m_son=val;
}

template<SubTheoremFormula VarType>
bool SubArithmeticTheorem<VarType>::isEqual(const ASubArithmeticRule<SubArithmeticTheorem::ValueType> &prop) const
{
    auto propCast=
            dynamic_cast<const SubArithmeticRule<VarType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<SubTheoremFormula VarType>
bool SubArithmeticTheorem<VarType>::isEqual(const ASubArithmeticTheorem<SubArithmeticTheorem::ValueType> &prop) const
{
    auto propCast=
            dynamic_cast<const SubArithmeticTheorem<VarType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<SubTheoremFormula VarType>
bool SubArithmeticTheorem<VarType>::operator==(const SubArithmeticTheorem &prop) const
{
    return *m_son==(prop.getSon());
}

template<SubTheoremFormula VarType>
bool SubArithmeticTheorem<VarType>::operator==(const SubArithmeticRule<SubArithmeticTheorem::SubOperatorType> &prop) const
{
    return *m_son==(prop.getSon());
}
}

#endif // DARK_LOGIC_VARIABLETHEOREMFORMULA_H
