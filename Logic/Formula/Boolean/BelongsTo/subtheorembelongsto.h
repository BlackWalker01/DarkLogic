#ifndef SUBTHEOREMBELONGSTO_H
#define SUBTHEOREMBELONGSTO_H
#include "Formula/Boolean/subtheorem.h"
#include "Formula/Boolean/asubimpuretheorem.h"

namespace N_Logic
{
template<typename SetType>
class SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>>: public ASubImpureTheorem
{
public:
    typedef BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>> SubPropertyType;

    SubTheorem(const std::string& name_, const ptr<ASubArithmeticTheorem<typename SetType::Type>>& leftFormula,
               const ptr<ASubArithmeticTheorem<SetType>>& rightFormula);
    SubTheorem(const std::string& name, const std::vector<ASubTheorem*>& subProps);
    SubTheorem(const SubRule<ConstBoolean>& prop);

    bool evaluate() const override final;

    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubTheorem& prop) const;
    bool isEqual(const ASubRule& prop) const override final;
    bool operator==(const SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;
    const std::vector<std::vector<Arity> > &computeAllPaths() override final;

    ptr<ASubTheorem> copyTheorem() const override final;
    const SubPropertyType& getSon() const;
    const ptr<IISubTheoremFormula>& operator[](const size_t& index) const override final;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, std::vector<size_t>& indexes, const size_t& actionKey) const override;

    ~SubTheorem() override = default;

protected:
    size_t arity() const override final;
    std::unique_ptr<SubPropertyType> m_son;
};

/*template<typename ValueType1, typename ValueType2>
SubTheorem<BelongsTo<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::SubTheorem(const SubRule<ConstBoolean> &prop)
{

}*/

template<typename SetType>
N_Logic::SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>> >::
SubTheorem(const std::string &name_, const ptr<ASubArithmeticTheorem<typename SetType::Type> > &leftFormula,
           const ptr<ASubArithmeticTheorem<SetType> > &rightFormula):
    ASubImpureTheorem(name_,leftFormula->getExtVars(),rightFormula->getExtVars(),PropType::BELONGSTO_PROP),
    m_son(std::make_unique<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>>(leftFormula,rightFormula))
{
    computeAllPaths();
}


template<typename SetType>
const typename SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType> > >::SubPropertyType&
SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType> > >::getSon() const
{
    return *(m_son.get());
}


template<typename SetType>
size_t N_Logic::SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType> > >::arity() const
{
    return m_son->arity();
}

template<typename SetType>
bool SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>> >::evaluate() const
{
    return m_son->evaluate();
}

template<typename SetType>
std::string N_Logic::SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>> >::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<typename SetType>
bool SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>> >::
isEqual(const ASubTheorem &prop) const
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
bool SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>> >::
operator==(const SubTheorem &prop) const
{
    return *m_son==(prop.getSon());
}

template<typename SetType>
bool N_Logic::SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>> >::
isEqual(const ASubRule &prop) const
{
    auto propCast=dynamic_cast<const SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,
            ASubArithmeticRule<SetType>>>*>(&prop);
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
bool N_Logic::SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>> >::
operator==(const SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>> &prop) const
{
    return *m_son==(prop.getSon());
}

template<typename SetType>
ptr<IISubTheoremFormula>
N_Logic::SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>> >::
ruleApply(const IISubRuleFormula &/*rule*/, std::vector<size_t> &indexes, const size_t &/*actionKey*/) const
{
    Arity index=indexes[0];
    indexes.erase(indexes.begin());
    if(index==0)
    {
        /*return new SubTheorem<SubPropertyType>(name(),
        {rule.applyAnnexe(actionKey,(*(*m_son)[0]),indexes), (*m_son)[1]->copy()});*/
    }
    else
    {
        /*return new SubTheorem<SubPropertyType>(name(),
        {(*m_son)[0]->copy(),rule.applyAnnexe(actionKey,(*(*m_son)[1]),indexes)});*/
    }
    return nullptr;
}

template<typename SetType>
const std::vector<std::vector<Arity> > &
SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>> >::computeAllPaths()
{
    if(!m_allPaths.size())
    {
        m_allPaths.push_back({});

        //left subProperty
        std::vector<std::vector<Arity>> leftPath=get<0>(*m_son)->getAllPaths();
        for(size_t k=0; k<leftPath.size();k++)
        {
            std::vector<Arity> crtPath=leftPath[k];
            crtPath.insert(crtPath.begin(),0);
            m_allPaths.push_back(crtPath);
        }

        //right subProperty
        std::vector<std::vector<Arity>> rightPath=get<1>(*m_son)->getAllPaths();
        for(size_t k=0; k<rightPath.size();k++)
        {
            std::vector<Arity> crtPath=rightPath[k];
            crtPath.insert(crtPath.begin(),1);
            m_allPaths.push_back(crtPath);
        }
    }
    return m_allPaths;
}

template<typename SetType>
const ptr<IISubTheoremFormula> &N_Logic::SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,
ASubArithmeticTheorem<SetType> > >::operator[](const size_t &index) const
{
    return (*m_son)[index];
}

}
#endif // SUBTHEOREMBELONGSTO_H
