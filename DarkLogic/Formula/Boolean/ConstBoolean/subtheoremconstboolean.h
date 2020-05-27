/*===--- subtheoremconstboolean.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubTheorem<ConstBoolean> in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/


#ifndef DARK_LOGIC_SUBTHEOREMCONSTBOOLEAN_H
#define DARK_LOGIC_SUBTHEOREMCONSTBOOLEAN_H
#include "Formula/Boolean/subtheorem.h"

namespace N_DarkLogic
{
template<>
class SubTheorem<ConstBoolean>: public ASubTheorem
{
public:
    typedef ConstBoolean SubPropertyType;

    SubTheorem(const bool& son);
    SubTheorem(const SubRule<ConstBoolean>& prop);

    bool isEvaluated() const override final;
    const State& getState() const override final;
    bool evaluate() const override final;
    constexpr PropType type() const override final
    {
        return PropType::CONST_PROP;
    }
    bool canBeDemonstrated() const override;

    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubTheorem& prop) const;
    bool isEqual(const ASubRule& prop) const override final;
    bool operator==(const SubRule<SubPropertyType>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;

    ptr<ASubTheorem> copyTheorem() const override final;
    const SubPropertyType& getSon() const;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
        std::vector<Arity>& indexes, const Action::Id& actionKey) const override;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
        std::vector<Arity>& indexes, const Action::Id& actionKey, const size_t& logicIdx) const override;

    ~SubTheorem() override = default;
protected:
    const std::vector<std::vector<Arity> > &computeAllPaths() override final;
    const std::vector<std::vector<Arity>>& computeImplPaths() override final;

protected:
    size_t arity() const override final;
    const std::unique_ptr<const SubPropertyType> m_son;
    const State m_state;
};
}
#endif // DARK_LOGIC_SUBTHEOREMCONSTBOOLEAN_H
