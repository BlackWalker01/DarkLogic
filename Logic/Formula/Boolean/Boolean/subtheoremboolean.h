#ifndef SUBTHEOREMBOOLEAN_H
#define SUBTHEOREMBOOLEAN_H
#include "Formula/Boolean/subtheorem.h"

namespace N_Logic
{
template<>
class SubTheorem<Boolean>: public ASubTheorem
{
public:
    typedef Boolean SubPropertyType;

    SubTheorem(const std::shared_ptr<Boolean>& val);

    bool evaluate() const override;
    constexpr PropType type() const override final
    {
        return PropType::VAR_PROP;
    }
    bool canBeDemonstrated() const override;

    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubTheorem& prop) const;
    bool isEqual(const ASubRule& prop) const override final;
    bool operator==(const SubRule<SubPropertyType>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;

    ptr<ASubTheorem> copyTheorem() const override final;
    const SubPropertyType& getSon() const;
    constexpr bool isHypProp() const override final
    {
        return m_son->isHypVariable();
    }
    IDVar variableId() const override final;

    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, std::vector<size_t>& indexes, const size_t& actionKey) const override;

    ~SubTheorem() override = default;
protected:
    const std::vector<std::vector<Arity> > &computeAllPaths() override final;
    const std::vector<std::vector<Arity>>& computeImplPaths() override final;

protected:
    size_t arity() const override final;

    const std::shared_ptr<SubPropertyType> m_son;
    const DbVar m_extVars;
};
}
#endif // SUBTHEOREMBOOLEAN_H
