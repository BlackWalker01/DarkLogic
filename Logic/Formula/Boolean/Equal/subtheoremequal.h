#ifndef SUBTHEOREMEQUAL_H
#define SUBTHEOREMEQUAL_H
#include "Formula/Boolean/subtheorem.h"
#include "Formula/Boolean/asubimpuretheorem.h"

namespace N_Logic
{
template<typename ValueType1, typename ValueType2>
class SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>>: public ASubImpureTheorem
{
public:
    typedef Equal<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>> SubPropertyType;

    SubTheorem(const ptr<ASubArithmeticTheorem<ValueType1>>& leftFormula,
               const ptr<ASubArithmeticTheorem<ValueType2>>& rightFormula);
    SubTheorem(const ptr<SubRule<ConstBoolean>>& prop);

    bool evaluate() const override final;
    constexpr PropType type() const override final;

    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubTheorem& prop) const;
    bool isEqual(const ASubRule& prop) const override final;
    bool operator==(const SubRule<Equal<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;
    const std::vector<std::vector<Arity> > &computeAllPaths() override final;
    const std::vector<std::vector<Arity>>& computeImplPaths() override final;

    ptr<ASubTheorem> copyTheorem() const override final;
    const SubPropertyType& getSon() const;
    const DbVar* getExtVars() const override final;
    const ptr<IISubTheoremFormula>& operator[](const size_t& index) const override final;

    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule,
                                       std::vector<size_t>& indexes, const size_t& actionKey) const override;

    ~SubTheorem() override = default;

protected:
    size_t arity() const override final;
    const std::unique_ptr<const SubPropertyType> m_son;
    const DbVar m_extVars;
};


template<typename ValueType1, typename ValueType2>
N_Logic::SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
SubTheorem(const ptr<ASubArithmeticTheorem<ValueType1>> &leftFormula,
           const ptr<ASubArithmeticTheorem<ValueType2>> &rightFormula):
    m_son(std::make_unique<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >>(leftFormula,rightFormula)),
    m_extVars(leftFormula->getExtVars(), rightFormula->getExtVars())
{
    computeAllPaths();
    computeImplPaths();
}

template<typename ValueType1, typename ValueType2>
const typename SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::SubPropertyType&
SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::getSon() const
{
    return *(m_son.get());
}

template<typename ValueType1, typename ValueType2>
size_t N_Logic::SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::arity() const
{
    return m_son->arity();
}

template<typename ValueType1, typename ValueType2>
bool SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::evaluate() const
{
    return m_son->evaluate();
}

template<typename ValueType1, typename ValueType2>
std::string N_Logic::SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<typename ValueType1, typename ValueType2>
const DbVar* N_Logic::SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::getExtVars() const
{
    return &m_extVars;
}

template<typename ValueType1, typename ValueType2>
constexpr IProposition::PropType SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::type() const
{
    return EQUAL_PROP;
}

template<typename ValueType1, typename ValueType2>
bool SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
isEqual(const ASubTheorem &prop) const
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

template<typename ValueType1, typename ValueType2>
bool SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
operator==(const SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > > &prop) const
{
    return *m_son==prop.getSon();
}

template<typename ValueType1, typename ValueType2>
bool N_Logic::SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
isEqual(const ASubRule &prop) const
{
    auto propCast=dynamic_cast<const SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> >>*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

template<typename ValueType1, typename ValueType2>
bool N_Logic::SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
operator==(const SubRule<Equal<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>> &prop) const
{
    return *m_son==prop.getSon();
}

template<typename ValueType1, typename ValueType2>
ptr<IISubTheoremFormula> N_Logic::SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
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

template<typename ValueType1, typename ValueType2>
const std::vector<std::vector<Arity> >&
N_Logic::SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::computeAllPaths()
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


template<typename ValueType1, typename ValueType2>
const std::vector<std::vector<Arity> >&
N_Logic::SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::computeImplPaths()
{
    if (!m_implPaths.size())
    {
        m_implPaths.push_back({});

        //left subProperty
        std::vector<std::vector<Arity>> leftPath = get<0>(*m_son)->getAllPaths();
        for (size_t k = 0; k < leftPath.size(); k++)
        {
            std::vector<Arity> crtPath = leftPath[k];
            crtPath.insert(crtPath.begin(), 0);
            m_implPaths.push_back(crtPath);
        }

        //right subProperty
        std::vector<std::vector<Arity>> rightPath = get<1>(*m_son)->getAllPaths();
        for (size_t k = 0; k < rightPath.size(); k++)
        {
            std::vector<Arity> crtPath = rightPath[k];
            crtPath.insert(crtPath.begin(), 1);
            m_implPaths.push_back(crtPath);
        }
    }
    return m_implPaths;
}


template<typename ValueType1, typename ValueType2>
const ptr<IISubTheoremFormula> &N_Logic::SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
operator[](const size_t &index) const
{
    return (*m_son)[index];
}

}
#endif // SUBTHEOREMEQUAL_H
