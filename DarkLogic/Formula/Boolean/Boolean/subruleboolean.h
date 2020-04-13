/*===--- subruleboolean.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubRule<Boolean> in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SUBRULEBOOLEAN_H
#define DARK_LOGIC_SUBRULEBOOLEAN_H
#include "Formula/Boolean/subrule.h"
#include "Variable/Boolean/boolean.h"

namespace N_DarkLogic
{
template<>
class SubRule<Boolean>: public ASubRule
{
public:
    typedef Boolean SubPropertyType;

    SubRule(const std::shared_ptr<Boolean>& val);

    bool evaluate() const override final;
    constexpr PropType type() const override final
    {
        return PropType::VAR_PROP;
    }

    bool isEqual(const ASubRule& prop) const override final;
    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubRule& prop) const;
    bool operator==(const SubTheorem<SubPropertyType>& prop) const;


    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;
    const SubPropertyType& getSon() const;
    constexpr bool isHypProp() const override final
    {
        return m_son->isHypVariable();
    }
    IDVar variableId() const override final;

    ~SubRule() override = default;

protected:
    bool identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyPriv(DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyFirstPriv(DbVarProp& dbVarProp) const override final;
    size_t arity() const override final;

protected:
    const std::shared_ptr<SubPropertyType> m_son;
    const DbVar m_extVars;
};
}

#endif // DARK_LOGIC_SUBRULEBOOLEAN_H
