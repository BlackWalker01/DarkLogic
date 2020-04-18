#ifndef THEOREMCONSTBOOLEAN_H
#define THEOREMCONSTBOOLEAN_H
#include "Formula/Boolean/theorem.h"

namespace N_Logic
{
template<>
class Theorem<ConstBoolean>: public SubTheorem<ConstBoolean>
{
public:
    typedef ConstBoolean SubPropertyType;

    Theorem(const bool& son);
    Theorem(const SubRule<ConstBoolean>& prop);


    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule,
                                       std::vector<size_t>& indexes, const size_t& actionKey) const override final;

    bool isEvaluated() const override final;

    ~Theorem() override = default;
};
}
#endif // THEOREMCONSTBOOLEAN_H
