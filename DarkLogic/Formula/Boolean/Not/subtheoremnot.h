/*===--- subtheoremnot.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubTheorem<Not<ASubTheorem>> in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SUBTHEOREMNOT_H
#define DARK_LOGIC_SUBTHEOREMNOT_H
#include "Formula/Boolean/subtheorem.h"
#include "Formula/Boolean/asubpuretheorem.h"

namespace N_DarkLogic
{
template<>
class SubTheorem<Not<ASubTheorem>>: public ASubPureTheorem
{
public:
    using SubPropertyType = Not<ASubTheorem>;
    using SubRulePropertyType = Not<ASubRule>;

    SubTheorem(const ptr<ASubTheorem>& subProp);

    bool evaluate() const override;
    constexpr PropType type() const override final
    {
        return PropType::NOT_PROP;
    }

    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubTheorem& prop) const;
    bool isEqual(const ASubRule& prop) const override final;
    bool operator==(const SubRule<SubRulePropertyType>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;

    ptr<ASubTheorem> copyTheorem() const override final;
    const SubPropertyType& getSon() const;
    const DbVar* getExtVars() const override final;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, std::vector<size_t>& indexes, const size_t& actionKey) const override;

    ~SubTheorem() override = default;
protected:
    const std::vector<std::vector<Arity> > &computeAllPaths() override final;
    const std::vector<std::vector<Arity>>& computeImplPaths() override final;

protected:
    const ptr<ASubTheorem>& operator[](const size_t& index) const override final;
    size_t arity() const override final;

    const std::unique_ptr<const SubPropertyType> m_son;
    const DbVar m_extVars;
};
}
#endif // DARK_LOGIC_SUBTHEOREMNOT_H
