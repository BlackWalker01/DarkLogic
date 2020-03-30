#ifndef SUBTHEOREMHYP_H
#define SUBTHEOREMHYP_H
#include "Formula/Boolean/subtheorem.h"
#include "Formula/Boolean/asubpuretheorem.h"

namespace N_Logic
{
template<>
class SubTheorem<Hyp<ASubTheorem>>: public ASubPureTheorem
{
public:
    typedef Hyp<ASubTheorem> SubPropertyType;

    SubTheorem(const std::string& name, const std::vector<ptr<ASubTheorem>>& subProps);

    bool evaluate() const override final;

    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubTheorem& prop) const;
    bool isEqual(const ASubRule& prop) const override final;
    bool operator==(const SubRule<Hyp<ASubRule>>& prop) const;

    size_t arity() const override final;
    std::string toString(unsigned short priorityParent=1000) const override final;

    ptr<ASubTheorem> copyTheorem() const override final;
    const SubPropertyType& getSon() const;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, std::vector<size_t>& indexes, const size_t& actionKey) const override;

    ~SubTheorem() override = default;
protected:
    const std::vector<std::vector<Arity> > &computeAllPaths() override final;

protected:
    const ptr<ASubTheorem>& operator[](const size_t& index) const override final;    

    const std::unique_ptr<const SubPropertyType> m_son;
};
}

#endif // SUBTHEOREMHYP_H
