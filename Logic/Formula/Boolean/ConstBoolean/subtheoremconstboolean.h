#ifndef SUBTHEOREMCONSTBOOLEAN_H
#define SUBTHEOREMCONSTBOOLEAN_H
#include "Formula/Boolean/subtheorem.h"

namespace N_Logic
{
template<>
class SubTheorem<ConstBoolean>: public ASubTheorem
{
public:
    typedef ConstBoolean SubPropertyType;

    SubTheorem(const std::string& name, const bool& son);
    SubTheorem(const SubRule<ConstBoolean>& prop);

    bool evaluate() const override final;
    bool canBeDemonstrated() const override;

    bool operator==(const ASubTheorem& prop) const override final;
    bool operator==(const SubTheorem& prop) const;
    bool operator==(const ASubRule& prop) const override final;
    bool operator==(const SubRule<SubPropertyType>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;

    ptr<ASubTheorem> copyTheorem() const override final;
    const SubPropertyType& getSon() const;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, std::vector<size_t>& indexes, const size_t& actionKey) const override;

    ~SubTheorem() override = default;
protected:
    const std::vector<std::vector<Arity> > &computeAllPaths() override final;

protected:
    size_t arity() const override final;
    const std::unique_ptr<const SubPropertyType> m_son;
};
}
#endif // SUBTHEOREMCONSTBOOLEAN_H
