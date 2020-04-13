/*===--- theoremconstboolean.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines Theorem<ConstBoolean> in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_THEOREMCONSTBOOLEAN_H
#define DARK_LOGIC_THEOREMCONSTBOOLEAN_H
#include "Formula/Boolean/theorem.h"

namespace N_DarkLogic
{
template<>
class Theorem<ConstBoolean>: public SubTheorem<ConstBoolean>
{
public:
    typedef ConstBoolean SubPropertyType;

    Theorem(const bool& son);
    Theorem(const SubRule<ConstBoolean>& prop);


    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule,
                                       std::vector<size_t>& indexes, const size_t& actionKey) const override final;

    bool isEvaluated() const override final;

    ~Theorem() override = default;
};
}
#endif // DARK_LOGIC_THEOREMCONSTBOOLEAN_H
