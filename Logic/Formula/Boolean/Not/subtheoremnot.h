#ifndef SUBTHEOREMNOT_H
#define SUBTHEOREMNOT_H
#include "Formula/Boolean/subtheorem.h"
#include "Formula/Boolean/asubpuretheorem.h"

namespace N_Logic
{
template<>
class SubTheorem<Not<ASubTheorem>>: public ASubPureTheorem
{
public:
    typedef Not<ASubTheorem> SubPropertyType;

    SubTheorem(const std::string& name, const ptr<ASubTheorem>& subProp);

    bool evaluate() const override final;

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
    const ptr<ASubTheorem>& operator[](const size_t& index) const override final;
    size_t arity() const override final;

    const std::unique_ptr<const SubPropertyType> m_son;
};
}
#endif // SUBTHEOREMNOT_H
