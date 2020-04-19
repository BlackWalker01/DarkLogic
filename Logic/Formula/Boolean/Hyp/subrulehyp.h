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

    SubRule(const std::vector<ptr<ASubRule>>& subProps);

    bool evaluate() const override final;
    constexpr PropType type() const override final
    {
        return PropType::HYP_PROP;
    }

    bool isEqual(const ASubRule& prop) const override final;
    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubRule& prop) const;
    bool operator==(const SubTheorem<Hyp<ASubTheorem>>& prop) const;

    std::vector<ptr<ASubRule>> getEquivalentRules() const override final;
    size_t arity() const override final;
    std::string toString(unsigned short priorityParent=1000) const override final;
    const SubPropertyType& getSon() const;
    const DbVar* getExtVars() const override final;

    ~SubRule() override = default;

protected:
    bool identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyPriv(DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyFirstPriv(DbVarProp& dbVarProp) const override final;
    const ptr<ASubRule>& operator[](const size_t& index) const override final;    

protected:
    const std::unique_ptr<const SubPropertyType> m_son;
    const DbVar m_extVars;
};
}

#endif // SUBRULEHYP_H
