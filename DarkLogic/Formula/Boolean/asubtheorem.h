/*===--- asubtheorem.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines in ASubTheorem class which is base class for all SubTheorems in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_ASUBTHEOREM_H
#define DARK_LOGIC_ASUBTHEOREM_H
#include "Operator/ioperator.h"
#include "iproposition.h"
#include "Formula/isubtheoremformula.h"
#include "Utils/action.h"
#include "Formula/evaluater.h"

namespace N_DarkLogic
{
class IISubRuleFormula;

class ASubTheorem: public IProposition, public ISubTheoremFormula<ASubTheorem>
{
public:
    ASubTheorem() = default;
    ASubTheorem(const ASubTheorem &prop) = delete;

    virtual ptr<ASubTheorem> copyTheorem() const =0; //method used by a subRule to create a theorem from this subTheorem
    const std::vector<std::vector<Arity> > &getAllPaths() const; //method to get all possible paths to terms of this subtheorem (once computed)
    virtual const std::vector<std::vector<Arity>>& computeAllPaths()=0; //method to compute all possible paths to terms of this subtheorem
    const std::vector<std::vector<Arity> >& getImplPaths() const; //method to get all possible paths for implications to terms of this subtheorem (once computed)
    virtual const std::vector<std::vector<Arity>>& computeImplPaths() = 0; //method to compute all possible paths to terms of this subtheorem
    virtual bool isEvaluated() const;
    virtual bool canBeDemonstrated() const;
    virtual inline bool testEvaluate(const Evaluater::ConfigEval& /*configEval*/) const
    {
        return evaluate();
    }
    virtual inline bool getHiddenValue() const
    {
        return evaluate();
    }
    virtual std::vector<Evaluater::ConfigEval> getCompatibleConfigs(const Evaluater::ConfigEval& commonConfig, 
        const std::unordered_map<IDVar, IDVar>& internalVars) const;
    virtual std::vector<std::pair<Evaluater::ConfigEval, bool>> getConfigEvals() const;
    virtual std::unordered_map<IDVar, IDVar> getVarToEval() const;


    //apply rule on subtheorem of this theorem following path from indexes
    virtual ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp, std::vector<Arity>& indexes,
                                               const Action::Id& actionKey) const =0;
    virtual ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp, std::vector<Arity>& indexes,
                                                const Action::Id& actionKey, const size_t& logicIdx) const = 0;

    virtual ~ASubTheorem() = default;

private:
    bool testCanBeDemonstrated(const std::vector<ptr<AVariable>>& vars, std::unique_ptr<bool>& crtValue) const;
protected:
    std::vector<std::vector<Arity>> m_allPaths;
    std::vector<std::vector<Arity>> m_implPaths;
};

template<> struct ToRuleStruct<ASubTheorem> { using Type = ASubRule; };
}
#endif // DARK_LOGIC_ASUBTHEOREM_H
