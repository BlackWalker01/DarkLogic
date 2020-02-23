#ifndef THEOREMNOT_H
#define THEOREMNOT_H
#include "Formula/Boolean/theorem.h"

namespace N_Logic
{
template<>
class Theorem<Not<ASubTheorem>>: public SubTheorem<Not<ASubTheorem>>
{
public:
    typedef Not<ASubTheorem> SubPropertyType;

    Theorem(const std::string& name_, const ptr<ASubTheorem>& subProp);


    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule,
                                       std::vector<size_t>& indexes, const size_t& actionKey) const override final;

    ~Theorem() override = default;
};
}
#endif // THEOREMNOT_H
