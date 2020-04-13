/*===--- theoremnot.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines Theorem<Not<ASubTheorem>> in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_THEOREMNOT_H
#define DARK_LOGIC_THEOREMNOT_H
#include "Formula/Boolean/theorem.h"

namespace N_DarkLogic
{
template<>
class Theorem<Not<ASubTheorem>>: public SubTheorem<Not<ASubTheorem>>
{
public:
    typedef Not<ASubTheorem> SubPropertyType;

    Theorem(const ptr<ASubTheorem>& subProp);


    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule,
                                       std::vector<size_t>& indexes, const size_t& actionKey) const override final;

    ~Theorem() override = default;

    bool isEvaluated() const override final;
    bool canBeDemonstrated() const override final;
    bool evaluate() const override final;
    bool testEvaluate() const override final;

private:
    void _evaluate() const;
    void _canBeDemonstrated() const;

    const std::unique_ptr<std::unique_ptr<bool>> m_value;
    const std::unique_ptr<std::unique_ptr<bool>> m_canBeDemonstrated;
};
}
#endif // DARK_LOGIC_THEOREMNOT_H
