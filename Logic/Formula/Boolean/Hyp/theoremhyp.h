#ifndef THEOREMHYP_H
#define THEOREMHYP_H
#include "Formula/Boolean/theorem.h"

namespace N_Logic
{
template<>
class Theorem<Hyp<ASubTheorem>>: public SubTheorem<Hyp<ASubTheorem>>
{
public:
    typedef Hyp<ASubTheorem> SubPropertyType;

    Theorem(const std::string& name_, const std::vector<ptr<ASubTheorem>>& subProps);


    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule,
                                       std::vector<size_t>& indexes, const size_t& actionKey) const override final;

    ~Theorem() override = default;
};
}
#endif // THEOREMHYP_H
