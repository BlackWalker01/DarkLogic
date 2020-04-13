/*===--- subrulelet.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubRule<Let<SubRuleType,ASubRule>> in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SUBRULELET_H
#define DARK_LOGIC_SUBRULELET_H
#include "Formula/Boolean/subrule.h"
#include "Operator/Boolean/let.h"
#include "Formula/Boolean/asubimpurerule.h"

namespace N_DarkLogic
{
template<typename SubRuleType>
class SubRule<Let<SubRuleType,ASubRule>>: public ASubImpureRule
{
public:
    typedef Let<SubRuleType,ASubRule> SubPropertyType;

    SubRule(const ptr<SubRuleType>& leftFormula,
            const ptr<ASubRule>& rightFormula);

    bool evaluate() const override final;
    constexpr PropType type() const override final;

    bool isEqual(const ASubRule& prop) const override final;
    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubRule& prop) const;
    template<typename SubTheoremType>
    bool operator==(const SubTheorem<Let<SubTheoremType, ASubTheorem>>& prop) const;

    std::vector<ptr<ASubRule>> getEquivalentRules() const override final;
    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;

    const SubPropertyType& getSon() const;
    const ptr<IISubRuleFormula>& operator[](const size_t& index) const override final;

    ~SubRule() override = default;

protected:
    bool identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyFirstPriv(DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyPriv(DbVarProp& dbVarProp) const override final;

    size_t arity() const override final;

protected:
    const std::unique_ptr<const SubPropertyType> m_son;
    const DbVar m_extVars;
};

template<typename SubRuleType>
SubRule<Let<SubRuleType, ASubRule > >::
SubRule(const ptr<SubRuleType>& leftFormula, const ptr<ASubRule>& rightFormula):
    m_son(std::make_unique<const Let<SubRuleType, ASubRule >>(leftFormula,rightFormula)),
    m_extVars(leftFormula->getExtVars(), rightFormula->getExtVars())
{

}

template<typename SubRuleType>
bool SubRule<Let<SubRuleType, ASubRule> >::evaluate() const
{
    return m_son->evaluate();
}

template<typename SubRuleType>
constexpr IProposition::PropType SubRule < Let<SubRuleType, ASubRule>>::type() const
{
    return LET_PROP;
}

template<typename SubRuleType>
bool SubRule<Let<SubRuleType, ASubRule > >::isEqual(const ASubRule &prop) const
{
    auto propCast=dynamic_cast<const SubRule<SubPropertyType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

template<typename SubRuleType>
bool SubRule<Let<SubRuleType, ASubRule > >::isEqual(const ASubTheorem &prop) const
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

template<typename SubRuleType>
std::vector<ptr<ASubRule>> SubRule<Let<SubRuleType, ASubRule > >::getEquivalentRules() const
{
    std::vector<ptr<ASubRule>> ret;
    std::vector<ptr<SubRuleType>> leftRet;
    leftRet.push_back(get<0>(*m_son));
    std::vector<ptr<ASubRule>> rightRet = get<1>(*m_son)->getEquivalentRules();
    rightRet.push_back(get<1>(*m_son));

    for (const auto& subRuleLeft : leftRet)
    {
        for (const auto& subRuleRight : rightRet)
        {
            ret.push_back(std::make_shared<const SubRule>(subRuleLeft, subRuleRight)); //direct case
        }
    }

    //remove last one because it is the same as "this" SubRuleFormula
    ret.pop_back();

    return ret;
}


template<typename SubRuleType>
bool SubRule<Let<SubRuleType, ASubRule > >::
operator==(const SubRule<Let<SubRuleType, ASubRule> > &prop) const
{
    return *m_son==prop.getSon();
}

template<typename SubRuleType> template<typename SubTheoremType>
bool SubRule<Let<SubRuleType, ASubRule> >::
operator==(const SubTheorem<Let<SubTheoremType, ASubTheorem>> &prop) const
{
    return *m_son==prop.getSon();
}

template<typename SubRuleType>
std::string SubRule<Let<SubRuleType, ASubRule> >::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<typename SubRuleType>
inline const DbVar* SubRule<Let<SubRuleType, ASubRule>>::getExtVars() const
{
    return &m_extVars;
}


template<typename SubRuleType>
const typename SubRule<Let<SubRuleType, ASubRule > >::SubPropertyType&
SubRule<Let<SubRuleType, ASubRule> >::getSon() const
{
    return *(m_son.get());
}

template<typename SubRuleType>
size_t SubRule<Let<SubRuleType, ASubRule > >::arity() const
{
    return m_son->arity();
}

template<typename SubRuleType>
const ptr<IISubRuleFormula> &N_DarkLogic::SubRule<Let<SubRuleType, ASubRule> >::operator[](const size_t &index) const
{
    return (*m_son)[index];
}

}

#endif // DARK_LOGIC_SUBRULELET_H
