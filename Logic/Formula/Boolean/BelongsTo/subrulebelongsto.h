#ifndef SUBRULEBELONGSTO_H
#define SUBRULEBELONGSTO_H
#include "Formula/Boolean/subrule.h"
#include "Operator/Boolean/belongsto.h"
#include "Formula/Boolean/asubimpurerule.h"

namespace N_Logic
{
template<typename SetType>
class SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>>: public ASubImpureRule
{
public:
    typedef BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>> SubPropertyType;

    SubRule(const std::string& name_, const ptr<ASubArithmeticRule<typename SetType::Type>>& leftFormula,
            const ptr<ASubArithmeticRule<SetType>>& rightFormula);

    bool evaluate() const override final;

    bool operator==(const ASubRule& prop) const override final;
    bool operator==(const ASubTheorem& prop) const override final;
    bool operator==(const SubRule& prop) const;
    bool operator==(const SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,
                    ASubArithmeticTheorem<SetType>>>& prop) const;


    std::string toString(unsigned short priorityParent=1000) const override final;

    const SubPropertyType& getSon() const;
    const ptr<IISubRuleFormula>& operator[](const size_t& index) const override final;

    ~SubRule() override = default;

protected:
    bool identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyFirstPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;
    size_t arity() const override final;

public:
    const std::unique_ptr<const SubPropertyType> m_son;
};

template<typename SetType>
SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>> >::
SubRule(const std::string &name_, const ptr<ASubArithmeticRule<typename SetType::Type>>& leftFormula,
        const ptr<ASubArithmeticRule<SetType>>& rightFormula):
    ASubImpureRule(name_,leftFormula->getExtVars(),rightFormula->getExtVars(),PropType::BELONGSTO_PROP),
    m_son(std::make_unique<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>>(leftFormula,rightFormula))
{

}

template<typename SetType>
bool SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>> >::evaluate() const
{
    return m_son->evaluate();
}

template<typename SetType>
bool SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>  >::
operator==(const ASubRule &prop) const
{
    auto propCast=dynamic_cast<const SubRule<SubPropertyType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<typename SetType>
bool SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>  >::
operator==(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem<SubPropertyType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<typename SetType>
bool SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>  >::
operator==(const SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,
           ASubArithmeticTheorem<SetType>>>& prop) const
{
    return *m_son==(prop.getSon());
}

template<typename SetType>
bool SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>   >::
operator==(const SubRule &prop) const
{
    return *m_son==(prop.getSon());
}

template<typename SetType>
std::string SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>> >::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}


template<typename SetType>
const typename SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>> >::SubPropertyType&
SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>> >::getSon() const
{
    return *(m_son.get());
}

template<typename SetType>
size_t SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>> >::arity() const
{
    return m_son->arity();
}

template<typename SetType>
const ptr<IISubRuleFormula> &N_Logic::SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>, ASubArithmeticRule<SetType> > >::
operator[](const size_t &index) const
{
    return (*m_son)[index];
}

}

#endif // SUBRULEBELONGSTO_H
