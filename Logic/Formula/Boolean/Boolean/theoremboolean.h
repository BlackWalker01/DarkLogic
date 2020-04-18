#ifndef THEOREMBOOLEAN_H
#define THEOREMBOOLEAN_H
#include "Formula/Boolean/theorem.h"

namespace N_Logic
{
template<>
class Theorem<Boolean>: public SubTheorem<Boolean>
{
public:
    typedef Boolean SubPropertyType;

    Theorem(const std::shared_ptr<Boolean>& son);

    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule,
                                       std::vector<size_t>& indexes, const size_t& actionKey) const override final;

    bool isEvaluated() const override final;

    ~Theorem() override = default;


};
}
#endif // THEOREMBOOLEAN_H
