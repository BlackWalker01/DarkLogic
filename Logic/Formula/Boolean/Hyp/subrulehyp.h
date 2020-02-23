#ifndef SUBRULEHYP_H
#define SUBRULEHYP_H
#include "Formula/Boolean/subrule.h"
#include "Operator/Boolean/hyp.h"
#include "Formula/Boolean/asubpurerule.h"

namespace N_Logic
{
template<>
class SubRule<Hyp<ASubRule>>: public ASubPureRule
{
public:
    typedef Hyp<ASubRule> SubPropertyType;

    SubRule(const std::string& name, const std::vector<ptr<ASubRule>>& subProps);

    bool evaluate() const override final;

    bool operator==(const ASubRule& prop) const override final;
    bool operator==(const ASubTheorem& prop) const override final;
    bool operator==(const SubRule& prop) const;
    bool operator==(const SubTheorem<Hyp<ASubTheorem>>& prop) const;


    size_t arity() const override final;
    std::string toString(unsigned short priorityParent=1000) const override final;
    const SubPropertyType& getSon() const;

    ~SubRule() override = default;

protected:
    bool identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyFirstPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;
    const ptr<ASubRule>& operator[](const size_t& index) const override final;    

protected:
    const std::unique_ptr<const SubPropertyType> m_son;
};
}

#endif // SUBRULEHYP_H
