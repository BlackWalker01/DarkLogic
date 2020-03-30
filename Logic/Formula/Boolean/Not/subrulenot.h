#ifndef SUBRULENOT_H
#define SUBRULENOT_H
#include "Formula/Boolean/subrule.h"
#include "Operator/Boolean/not.h"
#include "Formula/Boolean/asubpurerule.h"

namespace N_Logic
{
template<>
class SubRule<Not<ASubRule>>: public ASubPureRule
{
public:
    typedef Not<ASubRule> SubPropertyType;

    SubRule(const std::string& name, const ptr<ASubRule>& subProp);

    bool evaluate() const override final;

    bool isEqual(const ASubRule& prop) const override final;
    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubRule& prop) const;
    bool operator==(const SubTheorem<SubPropertyType>& prop) const;


    std::string toString(unsigned short priorityParent=1000) const override final;
    const SubPropertyType& getSon() const;

    ~SubRule() override = default;

protected:
    bool identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyFirstPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;
    const ptr<ASubRule>& operator[](const size_t& index) const override final;

    size_t arity() const override final;

protected:
    const std::unique_ptr<const SubPropertyType> m_son;
};
}

#endif // SUBRULENOT_H
