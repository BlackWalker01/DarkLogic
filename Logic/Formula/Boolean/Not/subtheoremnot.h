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
    using SubPropertyType = Not<ASubTheorem>;
    using SubRulePropertyType = Not<ASubRule>;

    SubTheorem(const ptr<ASubTheorem>& subProp);

    bool evaluate() const override final;
    constexpr PropType type() const override final;

    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubTheorem& prop) const;
    bool isEqual(const ASubRule& prop) const override final;
    bool operator==(const SubRule<SubRulePropertyType>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;

    ptr<ASubTheorem> copyTheorem() const override final;
    const SubPropertyType& getSon() const;
    const DbVar* getExtVars() const override final;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, std::vector<size_t>& indexes, const size_t& actionKey) const override;

    ~SubTheorem() override = default;
protected:
    const std::vector<std::vector<Arity> > &computeAllPaths() override final;

protected:
    const ptr<ASubTheorem>& operator[](const size_t& index) const override final;
    size_t arity() const override final;

    const std::unique_ptr<const SubPropertyType> m_son;
    const DbVar m_extVars;
};
}
#endif // SUBTHEOREMNOT_H
