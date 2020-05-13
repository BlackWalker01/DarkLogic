/*===--- subrulenot.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubRule<Not<ASubRule>> in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SUBRULENOT_H
#define DARK_LOGIC_SUBRULENOT_H
#include "Formula/Boolean/subrule.h"
#include "Operator/Boolean/not.h"
#include "Formula/Boolean/asubpurerule.h"

namespace N_DarkLogic
{
template<>
class SubRule<Not<ASubRule>>: public ASubPureRule
{
public:
    typedef Not<ASubRule> SubPropertyType;

    SubRule(const ptr<ASubRule>& subProp);

    bool evaluate() const override final;
    constexpr PropType type() const override final
    {
        return PropType::NOT_PROP;
    }

    bool isEqual(const ASubRule& prop) const override final;
    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubRule& prop) const;
    bool operator==(const SubTheorem<ToTheoremOpe<SubPropertyType>>& prop) const;

    std::vector<ptr<ASubRule>> getEquivalentRules() const override final;
    std::string toString(unsigned short priorityParent=1000) const override final;
    const SubPropertyType& getSon() const;
    const DbVar* getExtVars() const override final;

    ~SubRule() override = default;

protected:
    bool identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const override final;
    ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const override final;
    const ptr<ASubRule>& operator[](const size_t& index) const override final;

    size_t arity() const override final;

protected:
    const std::unique_ptr<const SubPropertyType> m_son;
    const DbVar m_extVars;
};
}

#endif // DARK_LOGIC_SUBRULENOT_H
