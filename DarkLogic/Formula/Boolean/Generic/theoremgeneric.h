/*===--- theoremgeneric.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines Theorem generic case in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_THEOREMGENERIC_H
#define DARK_LOGIC_THEOREMGENERIC_H
#include "Formula/Boolean/theorem.h"

namespace N_DarkLogic
{
template<SubTheoremProperty SubPropertyType>
class Theorem: public SubTheorem<SubPropertyType>
{
public:
    Theorem(const ptr<ASubTheorem>& leftSubProp, const ptr<ASubTheorem>& rightSubProp);


    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
        std::vector<Arity>& indexes, const Action::Id& actionKey) const override final;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
        std::vector<Arity>& indexes, const Action::Id& actionKey, const size_t& logicIdx) const override final;

    ~Theorem() override = default;
};

template<SubTheoremProperty SubPropertyType> struct ToRuleStruct<Theorem<SubPropertyType>> { using Type = Rule<SubPropertyType>; };


template<SubTheoremProperty SubPropertyType>
Theorem<SubPropertyType>::Theorem(const ptr<ASubTheorem> &leftSubProp, const ptr<ASubTheorem> &rightSubProp):
    SubTheorem<SubPropertyType>(leftSubProp,rightSubProp)
{
    
}

template<SubTheoremProperty SubPropertyType>
ptr<IISubTheoremFormula> N_DarkLogic::Theorem<SubPropertyType>::ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
    std::vector<Arity>& indexes, const Action::Id& actionKey) const
{
    Arity index = indexes[0];
    indexes.erase(indexes.begin());
    if (index == 0)
    {
        return Logic::make_theorem_formula<Theorem<SubPropertyType>>(
            std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey,
                (*this->m_son)[0], dbVarProp, indexes)), (*this->m_son)[1]);
    }
    else
    {
        return Logic::make_theorem_formula<Theorem<SubPropertyType>>(
            (*this->m_son)[0], std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey,
                (*this->m_son)[1], dbVarProp, indexes)));
    }
}

template<SubTheoremProperty SubPropertyType>
ptr<IISubTheoremFormula> N_DarkLogic::Theorem<SubPropertyType>::ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
    std::vector<Arity>& indexes, const Action::Id& actionKey, const size_t& logicIdx) const
{
    Arity index = indexes[0];
    indexes.erase(indexes.begin());
    if (index == 0)
    {
        return Logic::make_theorem_formula<Theorem<SubPropertyType>>(logicIdx,
            std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey,
                (*this->m_son)[0], dbVarProp, indexes, logicIdx)), (*this->m_son)[1]);
    }
    else
    {
        return Logic::make_theorem_formula<Theorem<SubPropertyType>>(logicIdx,
            (*this->m_son)[0], std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey,
                (*this->m_son)[1], dbVarProp, indexes, logicIdx)));
    }
}
}

#endif // DARK_LOGIC_THEOREMGENERIC_H
