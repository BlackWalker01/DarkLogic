/*===--- subrulegeneric.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubRule generic case in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SUBRULEGENERIC_H
#define DARK_LOGIC_SUBRULEGENERIC_H
#include "Formula/Boolean/subrule.h"
#include "Formula/Boolean/asubpurerule.h"
#include "Operator/Boolean/and.h"
#include "Operator/Boolean/equivalent.h"
#include "Operator/Boolean/implication.h"
#include "Operator/Boolean/or.h"
#include "logic.h"
#include "Formula/Boolean/Generic/theoremgeneric.h"

namespace N_DarkLogic
{
template<SubRuleProperty SubPropertyType_>
class SubRule: public ASubPureRule
{
public:
    using SubPropertyType = SubPropertyType_;

    SubRule(const ptr<ASubRule>& leftSubProp, const ptr<ASubRule>& rightSubProp);

    bool evaluate() const override final;
    const State& getState() const override final;
    constexpr PropType type() const override final
    {
        if constexpr (std::is_same_v<SubPropertyType, And<ASubRule>>)
        {
            return AND_PROP;
        }
        else if constexpr (std::is_same_v<SubPropertyType, Equivalent<ASubRule>>)
        {
            return EQUIV_PROP;
        }
        else if constexpr (std::is_same_v<SubPropertyType, Implication<ASubRule>>)
        {
            return IMPL_PROP;
        }
        else if constexpr (std::is_same_v<SubPropertyType, Not<ASubRule>>)
        {
            return NOT_PROP;
        }
        else if constexpr (std::is_same_v<SubPropertyType, Or<ASubRule>>)
        {
            return OR_PROP;
        }
    }

    bool isEqual(const ASubRule& prop) const override final;
    bool isEqual(const ASubTheorem& prop) const override;
    bool operator==(const SubRule& prop) const;
    bool operator==(const SubTheorem<ToTheoremOpe<SubPropertyType>>& prop) const;

    std::vector<ptr<ASubRule>> getEquivalentRules() const override;
    std::string toString(unsigned short priorityParent=1000) const override final;
    const SubPropertyType& getSon() const;
    const DbVar* getExtVars() const override final;

    ~SubRule() override = default;

protected:
    bool identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyPriv(DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const override final;
    ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const override final;

    const ptr<ASubRule>& operator[](const size_t& index) const override final;
    size_t arity() const override final;

protected:
    const std::unique_ptr<const SubPropertyType> m_son;
    const DbVar m_extVars;
    const State m_state;
};

template<SubRuleProperty SubPropertyType> 
struct ToTheoremStruct<SubRule<SubPropertyType>> { using Type = SubTheorem<SubPropertyType>; };

/**
 * IdentifyPriv methods
 */
template<>
bool SubRule<And<ASubRule>>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const;

template<>
bool SubRule<Equivalent<ASubRule>>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const;

template<>
bool SubRule<Implication<ASubRule>>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const;

template<>
bool SubRule<Or<ASubRule>>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const;




template<SubRuleProperty SubPropertyType>
ptr<ASubTheorem> SubRule<SubPropertyType>::applyPriv(DbVarProp& dbVarProp) const
{
    return Logic::make_theorem_formula<SubTheorem<ToTheoremOpe<SubPropertyType>>>((*m_son)[0]->applyPriv(dbVarProp),
        (*m_son)[1]->applyPriv(dbVarProp));
}

template<SubRuleProperty SubPropertyType>
ptr<ASubTheorem> SubRule<SubPropertyType>::applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<SubTheorem<ToTheoremOpe<SubPropertyType>>>(logicIdx, (*m_son)[0]->applyPriv(dbVarProp, logicIdx),
        (*m_son)[1]->applyPriv(dbVarProp, logicIdx));
}

template<SubRuleProperty SubPropertyType>
ptr<ASubTheorem> SubRule<SubPropertyType>::applyFirstPriv(DbVarProp& dbVarProp) const
{
    return Logic::make_theorem_formula<Theorem<ToTheoremOpe<SubPropertyType>>>((*m_son)[0]->applyPriv(dbVarProp),
        (*m_son)[1]->applyPriv(dbVarProp));
}

template<SubRuleProperty SubPropertyType>
ptr<ASubTheorem> SubRule<SubPropertyType>::applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<Theorem<ToTheoremOpe<SubPropertyType>>>(logicIdx, (*m_son)[0]->applyPriv(dbVarProp, logicIdx),
        (*m_son)[1]->applyPriv(dbVarProp, logicIdx));
}

template<SubRuleProperty SubPropertyType>
const DbVar* SubRule<SubPropertyType>::getExtVars() const
{
    return &m_extVars;
}
}

#endif // DARK_LOGIC_SUBRULEGENERIC_H
