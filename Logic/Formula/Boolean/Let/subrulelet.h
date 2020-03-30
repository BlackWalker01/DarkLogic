#ifndef SUBRULELET_H
#define SUBRULELET_H
#include "Formula/Boolean/subrule.h"
#include "Operator/Boolean/let.h"
#include "Formula/Boolean/asubimpurerule.h"

namespace N_Logic
{
template<typename SubRuleType>
class SubRule<Let<SubRuleType,ASubRule>>: public ASubImpureRule
{
public:
    typedef Let<SubRuleType,ASubRule> SubPropertyType;

    SubRule(const std::string& name_, const ptr<SubRuleType>& leftFormula,
            const ptr<ASubRule>& rightFormula);

    bool evaluate() const override final;

    bool isEqual(const ASubRule& prop) const override final;
    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubRule& prop) const;
    template<typename SubTheoremType>
    bool operator==(const SubTheorem<Let<SubTheoremType, ASubTheorem>>& prop) const;


    std::string toString(unsigned short priorityParent=1000) const override final;

    const SubPropertyType& getSon() const;
    const ptr<IISubRuleFormula>& operator[](const size_t& index) const override final;

    ~SubRule() override = default;

protected:
    bool identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyFirstPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;

    size_t arity() const override final;

protected:
    const std::unique_ptr<const SubPropertyType> m_son;
};

template<typename SubRuleType>
SubRule<Let<SubRuleType, ASubRule > >::
SubRule(const std::string &name_, const ptr<SubRuleType>& leftFormula, const ptr<ASubRule>& rightFormula):
    ASubImpureRule(name_,leftFormula->getExtVars(),rightFormula->getExtVars(),PropType::LET_PROP),
    m_son(std::make_unique<const Let<SubRuleType, ASubRule >>(leftFormula,rightFormula))
{

}

template<typename SubRuleType>
bool SubRule<Let<SubRuleType, ASubRule> >::evaluate() const
{
    return m_son->evaluate();
}

template<typename SubRuleType>
bool SubRule<Let<SubRuleType, ASubRule > >::isEqual(const ASubRule &prop) const
{
    auto propCast=dynamic_cast<const SubRule<SubPropertyType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

template<typename SubRuleType>
bool SubRule<Let<SubRuleType, ASubRule > >::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem<SubPropertyType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

template<typename SubRuleType>
bool SubRule<Let<SubRuleType, ASubRule > >::
operator==(const SubRule<Let<SubRuleType, ASubRule> > &prop) const
{
    return *m_son==prop.getSon();
}

template<typename SubRuleType> template<typename SubTheoremType>
bool SubRule<Let<SubRuleType, ASubRule> >::
operator==(const SubTheorem<Let<SubTheoremType, ASubTheorem>> &prop) const
{
    return *m_son==prop.getSon();
}

template<typename SubRuleType>
std::string SubRule<Let<SubRuleType, ASubRule> >::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}


template<typename SubRuleType>
const typename SubRule<Let<SubRuleType, ASubRule > >::SubPropertyType&
SubRule<Let<SubRuleType, ASubRule> >::getSon() const
{
    return *(m_son.get());
}

template<typename SubRuleType>
size_t SubRule<Let<SubRuleType, ASubRule > >::arity() const
{
    return m_son->arity();
}

template<typename SubRuleType>
const ptr<IISubRuleFormula> &N_Logic::SubRule<Let<SubRuleType, ASubRule> >::operator[](const size_t &index) const
{
    return (*m_son)[index];
}

}

#endif // SUBRULELET_H
