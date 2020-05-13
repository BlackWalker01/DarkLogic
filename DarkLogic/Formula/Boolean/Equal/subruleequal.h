/*===--- subruleequal.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubRule<Equal> in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SUBRULEEQUAL_H
#define DARK_LOGIC_SUBRULEEQUAL_H
#include "Formula/Boolean/subrule.h"
#include "Operator/Boolean/equal.h"
#include "Formula/Boolean/asubimpurerule.h"

namespace N_DarkLogic
{
template<typename ValueType1, typename ValueType2>
class SubRule<Equal<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>>: public ASubImpureRule
{
public:
    typedef Equal<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>> SubPropertyType;

    SubRule(const ptr<ASubArithmeticRule<ValueType1>>& leftFormula,
            const ptr<ASubArithmeticRule<ValueType1>>& rightFormula);

    bool evaluate() const override final;
    constexpr PropType type() const override final;

    bool isEqual(const ASubRule& prop) const override final;
    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubRule& prop) const;
    bool operator==(const SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2>>>& prop) const;

    std::vector<ptr<ASubRule>> getEquivalentRules() const override final;
    std::string toString(unsigned short priorityParent=1000) const override final;

    const SubPropertyType& getSon() const;
    const DbVar* getExtVars() const override final;
    const ptr<IISubRuleFormula>& operator[](const size_t& index) const override final;

    ~SubRule() override = default;

protected:
    bool identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const override final;
    ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const override final;
    size_t arity() const override final;

protected:
    const std::unique_ptr<const SubPropertyType> m_son;
    const DbVar m_extVars;
};

template<typename ValueType1, typename ValueType2>
SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
SubRule(const ptr<ASubArithmeticRule<ValueType1>> &leftFormula,
        const ptr<ASubArithmeticRule<ValueType1>> &rightFormula):
    m_son(std::make_unique<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> >>(leftFormula,rightFormula)),
    m_extVars(leftFormula->getExtVars(), rightFormula->getExtVars())
{

}

template<typename ValueType1, typename ValueType2>
bool SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::evaluate() const
{
    return m_son->evaluate();
}

template<typename ValueType1, typename ValueType2>
bool SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::isEqual(const ASubRule &prop) const
{
    auto propCast=dynamic_cast<const SubRule<SubPropertyType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<typename ValueType1, typename ValueType2>
bool SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem<ToTheoremOpe<SubPropertyType>>*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

template<typename ValueType1, typename ValueType2>
bool SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
operator==(const SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> >> &prop) const
{
    return *m_son==prop.getSon();
}

template<typename ValueType1, typename ValueType2>
bool SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
operator==(const SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2>>> &prop) const
{
    return *m_son==prop.getSon();
}

template<typename ValueType1, typename ValueType2>
std::string SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<typename ValueType1, typename ValueType2>
const DbVar* SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::getExtVars() const
{
    return &m_extVars;
}


template<typename ValueType1, typename ValueType2>
std::vector<ptr<ASubRule>> SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
getEquivalentRules() const
{
    std::vector<ptr<ASubRule>> ret;
    std::vector<ptr<ASubArithmeticRule<ValueType1>>> leftRet = get<0>(*m_son)->getEquivalentRules();
    leftRet.push_back(get<0>(*m_son));
    std::vector<ptr<ASubArithmeticRule<ValueType2>>> rightRet = get<1>(*m_son)->getEquivalentRules();
    rightRet.push_back(get<1>(*m_son));

    for (const auto& subRuleLeft : leftRet)
    {
        for (const auto& subRuleRight : rightRet)
        {
            ret.push_back(std::make_shared<const SubRule<Equal<ASubArithmeticRule<ValueType2>,
                ASubArithmeticRule<ValueType1>>>>(subRuleRight, subRuleLeft)); //symmetric case
            ret.push_back(std::make_shared<const SubRule>(subRuleLeft, subRuleRight)); //direct case
        }
    }

    //remove last one because it is the same as "this" SubRuleFormula
    ret.pop_back();

    return ret;
}

template<typename ValueType1, typename ValueType2>
const typename SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::SubPropertyType&
SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::getSon() const
{
    return *(m_son.get());
}

template<typename ValueType1, typename ValueType2>
constexpr IProposition::PropType SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::type() const
{
    return EQUAL_PROP;
}

template<typename ValueType1, typename ValueType2>
size_t SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::arity() const
{
    return m_son->arity();
}

template<typename ValueType1, typename ValueType2>
const ptr<IISubRuleFormula> &N_DarkLogic::SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
operator[](const size_t &index) const
{
    return (*m_son)[index];
}

}

#endif // DARK_LOGIC_SUBRULEEQUAL_H
