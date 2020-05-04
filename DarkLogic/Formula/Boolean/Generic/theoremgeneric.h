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


    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, std::vector<Arity>& indexes,
                                       const Action::Id& actionKey) const override final;

    ~Theorem() override = default;
};

template<SubTheoremProperty SubPropertyType> struct ToRuleStruct<Theorem<SubPropertyType>> { using Type = Rule<SubPropertyType>; };


template<SubTheoremProperty SubPropertyType>
Theorem<SubPropertyType>::Theorem(const ptr<ASubTheorem> &leftSubProp, const ptr<ASubTheorem> &rightSubProp):
    SubTheorem<SubPropertyType>(leftSubProp,rightSubProp)
{
    
}
}

#endif // DARK_LOGIC_THEOREMGENERIC_H
