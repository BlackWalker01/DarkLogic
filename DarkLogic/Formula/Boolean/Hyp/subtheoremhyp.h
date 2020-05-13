/*===--- subtheoremhyp.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubTheorem<Hyp<ASubTheorem>> in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SUBTHEOREMHYP_H
#define DARK_LOGIC_SUBTHEOREMHYP_H
#include "Formula/Boolean/subtheorem.h"
#include "Formula/Boolean/asubpuretheorem.h"

namespace N_DarkLogic
{
template<>
class SubTheorem<Hyp<ASubTheorem>>: public ASubPureTheorem
{
public:
    typedef Hyp<ASubTheorem> SubPropertyType;

    SubTheorem(const std::vector<ptr<ASubTheorem>>& subProps);

    //evaluation methods
    bool isEvaluated() const override final;
    bool evaluate() const override final;
    bool canBeDemonstrated() const override final;
    bool testEvaluate(const Evaluater::ConfigEval& configEval) const override final;
    bool getHiddenValue() const override final;
    std::unordered_map<IDVar, IDVar> getVarToEval() const override final;
    std::vector<std::pair<Evaluater::ConfigEval, bool>> getConfigEvals() const override final;
    std::vector<Evaluater::ConfigEval> getCompatibleConfigs(const Evaluater::ConfigEval& commonConfig,
        const std::unordered_map<IDVar, IDVar>& internalVars) const override final;

    constexpr PropType type() const override final
    {
        return PropType::HYP_PROP;
    }

    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubTheorem& prop) const;
    bool isEqual(const ASubRule& prop) const override final;
    bool operator==(const SubRule<Hyp<ASubRule>>& prop) const;

    size_t arity() const override final;
    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;

    ptr<ASubTheorem> copyTheorem() const override final;
    const SubPropertyType& getSon() const;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
        std::vector<Arity>& indexes, const Action::Id& actionKey) const override;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
        std::vector<Arity>& indexes, const Action::Id& actionKey, const size_t& logicIdx) const override;

    ~SubTheorem() override = default;
private:
    void initEval();
protected:
    const std::vector<std::vector<Arity> > &computeAllPaths() override final;
    const std::vector<std::vector<Arity>>& computeImplPaths() override final;
    const ptr<ASubTheorem>& operator[](const size_t& index) const override final;    

    const std::unique_ptr<const SubPropertyType> m_son;
    const DbVar m_extVars;
    const std::unique_ptr<Evaluater> m_eval;
};
}

#endif // DARK_LOGIC_SUBTHEOREMHYP_H
