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

    SubRule(const ptr<ASubArithmeticRule<typename SetType::Type>>& leftFormula,
            const ptr<ASubArithmeticRule<SetType>>& rightFormula);

    bool evaluate() const override final;
    constexpr PropType type() const override final;

    bool isEqual(const ASubRule& prop) const override final;
    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubRule& prop) const;
    bool operator==(const SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,
                    ASubArithmeticTheorem<SetType>>>& prop) const;

    std::vector<ptr<ASubRule>> getEquivalentRules() const override final;
    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;
    const SubPropertyType& getSon() const;
    const ptr<IISubRuleFormula>& operator[](const size_t& index) const override final;

    ~SubRule() override = default;

protected:
    bool identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyFirstPriv(DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyPriv(DbVarProp& dbVarProp) const override final;
    size_t arity() const override final;

public:
    const std::unique_ptr<const SubPropertyType> m_son;
    const DbVar m_extVars;
};

template<typename SetType>
SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>> >::
SubRule(const ptr<ASubArithmeticRule<typename SetType::Type>>& leftFormula,
        const ptr<ASubArithmeticRule<SetType>>& rightFormula):
    m_son(std::make_unique<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>>(leftFormula,rightFormula)),
    m_extVars(leftFormula->getExtVars(), rightFormula->getExtVars())
{

}

template<typename SetType>
bool SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>> >::evaluate() const
{
    return m_son->evaluate();
}

template<typename SetType>
inline constexpr IProposition::PropType SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>, ASubArithmeticRule<SetType>>>::type() const
{
    return BELONGSTO_PROP;
}

template<typename SetType>
bool SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>  >::
isEqual(const ASubRule &prop) const
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
isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem<ToTheoremOpe<SubPropertyType>>*>(&prop);
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
inline std::vector<ptr<ASubRule>> SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>, ASubArithmeticRule<SetType>>>::getEquivalentRules() const
{
    std::vector<ptr<ASubRule>> ret;
    std::vector<ptr<ASubArithmeticRule<typename SetType::Type>>> leftRet;
    leftRet.push_back(get<0>(*m_son));
    std::vector<ptr<ASubArithmeticRule<SetType>>> rightRet = get<1>(*m_son)->getEquivalentRules();
    rightRet.push_back(get<1>(*m_son));

    for (const auto& subRuleLeft : leftRet)
    {
        for (const auto& subRuleRight : rightRet)
        {
            ret.push_back(std::make_shared<const SubRule>(subRuleLeft, subRuleRight)); //direct case
        }
    }

    //remove last one because it is the same as "this" SubRuleFormula
    ret.pop_back();

    return ret;
}

template<typename SetType>
std::string SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>> >::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<typename SetType>
const DbVar* SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>, ASubArithmeticRule<SetType>> >::getExtVars() const
{
    return &m_extVars;
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
