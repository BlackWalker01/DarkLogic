/*===--- subruleconstboolean.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubRule<ConstBoolean> in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SUBRULECONSTBOOLEAN_H
#define DARK_LOGIC_SUBRULECONSTBOOLEAN_H
#include "Formula/Boolean/subrule.h"
#include "Variable/Boolean/constboolean.h"

namespace N_DarkLogic
{
template<>
class SubRule<ConstBoolean>: public ASubRule
{
public:
    typedef ConstBoolean SubPropertyType;

    SubRule(const bool& son);

    bool evaluate() const override final;
    constexpr PropType type() const override final
    {
        return PropType::CONST_PROP;
    }

    bool isEqual(const ASubRule& prop) const override final;
    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubRule& prop) const;
    bool operator==(const SubTheorem<SubPropertyType>& prop) const;


    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;
    const SubPropertyType& getSon() const;

    ~SubRule() override = default;

protected:
    bool identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const override final;
    ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const override final;
    size_t arity() const override final;

protected:
    const std::unique_ptr<const SubPropertyType> m_son;
};
}

#endif // DARK_LOGIC_SUBRULECONSTBOOLEAN_H
