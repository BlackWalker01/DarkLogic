#ifndef THEOREMGENERIC_H
#define THEOREMGENERIC_H
#include "Formula/Boolean/theorem.h"

namespace N_Logic
{
template<SubTheoremProperty SubPropertyType>
class Theorem: public SubTheorem<SubPropertyType>
{
public:
    Theorem(const std::string& name_, const ptr<ASubTheorem>& leftSubProp, const ptr<ASubTheorem>& rightSubProp);


    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, std::vector<size_t>& indexes,
                                       const size_t& actionKey) const override final;

    ~Theorem() override = default;
};


template<SubTheoremProperty SubPropertyType>
Theorem<SubPropertyType>::Theorem(const std::string &name_, const ptr<ASubTheorem> &leftSubProp, const ptr<ASubTheorem> &rightSubProp):
    SubTheorem<SubPropertyType>(name_,leftSubProp,rightSubProp)
{

}

}

#endif // THEOREMGENERIC_H
