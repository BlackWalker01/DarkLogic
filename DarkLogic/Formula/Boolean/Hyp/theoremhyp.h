/*===--- theoremhyp.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines Theorem<Hyp<ASubTheorem>> in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_THEOREMHYP_H
#define DARK_LOGIC_THEOREMHYP_H
#include "Formula/Boolean/theorem.h"

namespace N_DarkLogic
{
template<>
class Theorem<Hyp<ASubTheorem>>: public SubTheorem<Hyp<ASubTheorem>>
{
public:
    typedef Hyp<ASubTheorem> SubPropertyType;

    Theorem(const std::vector<ptr<ASubTheorem>>& subProps);


    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
        std::vector<Arity>& indexes, const Action::Id& actionKey) const override final;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
        std::vector<Arity>& indexes, const Action::Id& actionKey, const size_t& logicIdx) const override final;

    ~Theorem() override = default;
};
}
#endif // DARK_LOGIC_THEOREMHYP_H
