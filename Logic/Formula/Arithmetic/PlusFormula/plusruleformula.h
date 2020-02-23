#ifndef PLUSRULEFORMULA_H
#define PLUSRULEFORMULA_H
#include "Formula/Arithmetic/subarithmeticrule.h"
#include "Operator/Arithmetic/plus.h"
#include "Formula/Arithmetic/inontermarithmetic.h"

namespace N_Logic
{

template<typename ValueType1, typename ValueType2>
class SubArithmeticRule<Plus< ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>>:
        public ASubArithmeticRule<typename Plus< ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>::ValueType>,
        public INonTermArithmetic<IISubRuleFormula>
{
public:
    typedef ASubArithmeticRule<ValueType1> SubLeftFormula;
    typedef ASubArithmeticRule<ValueType2> SubRightFormula;
    typedef Plus< SubLeftFormula,SubRightFormula> SubOperatorType;
    typedef typename SubOperatorType::ValueType ValueType;
    typedef typename
    ASubArithmeticRule<typename Plus< ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>::ValueType>::ATheoremType ATheoremType;

    SubArithmeticRule(const std::string& name_, const ptr<SubLeftFormula>& leftFormula, const ptr<SubRightFormula>& rightFormula);

    size_t arity() const override final;
    ValueType evaluate() const override final;

    //internal methods
    bool identifyPriv(const ptr<ATheoremType>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;

    bool operator==(const ASubArithmeticRule<ValueType>& prop) const override final;
    bool operator==(const ASubArithmeticTheorem<ValueType>& prop) const override final;
    bool operator==(const SubArithmeticRule& prop) const;
    bool operator==(const SubArithmeticTheorem<SubOperatorType>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;

    const SubOperatorType& getSon() const;
    const ptr<IISubRuleFormula>& operator[](const size_t& k) const override final;

    ~SubArithmeticRule() override = default;

protected:
    const std::unique_ptr<const SubOperatorType> m_son;
};

template<typename ValueType1, typename ValueType2>
SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
SubArithmeticRule(const std::string& name_,
                  const ptr<SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::SubLeftFormula> &leftFormula,
                  const ptr<SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::SubRightFormula> &rightFormula):
    ASubArithmeticRule<typename Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> >::ValueType>(name_,PLUS_FORMULA),
    m_son(std::make_unique<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2>> >(leftFormula,rightFormula))
{

}


template<typename ValueType1, typename ValueType2>
size_t SubArithmeticRule< Plus< ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2> > >::arity() const
{
    return m_son->arity();
}


template<typename ValueType1, typename ValueType2>
bool SubArithmeticRule< Plus< ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2> > >::
operator==(const ASubArithmeticRule<ValueType>& prop) const
{
    auto propCast=dynamic_cast<const SubArithmeticRule*>(&prop);
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
bool SubArithmeticRule< Plus< ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2> > >::
operator==(const ASubArithmeticTheorem<ValueType>& prop) const
{
    auto propCast=dynamic_cast<const SubArithmeticTheorem<Plus< ASubArithmeticTheorem<ValueType1>,
            ASubArithmeticTheorem<ValueType2> >>*>(&prop);
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
bool SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
operator==(const SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > > &prop) const
{
    return *m_son==prop->getSon();
}

template<typename ValueType1, typename ValueType2>
bool SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::operator==
(const SubArithmeticTheorem<SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::SubOperatorType> &prop) const
{
    return *m_son==(prop.getSon());
}

template<typename ValueType1, typename ValueType2>
bool SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
identifyPriv(const ptr<SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::ATheoremType> &prop,
             DbVarProp &dbVarProp) const
{
    auto propCast=std::dynamic_pointer_cast<const SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>,
            ASubArithmeticTheorem<ValueType2> > >>(prop);
    if(propCast)
    {
        return get<0>(*m_son)->identifyPriv(get<0>(propCast->getSon()),dbVarProp) &&
                get<1>(*m_son)->identifyPriv(get<1>(propCast->getSon()),dbVarProp);
    }
    return false;
}

template<typename ValueType1, typename ValueType2>
ptr<typename SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::ATheoremType>
SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
applyPriv(const std::string &thName, DbVarProp &dbVarProp) const
{
    return std::make_shared<const SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >>>(thName,
    get<0>(*m_son)->applyPriv(thName+"+R",dbVarProp),get<1>(*m_son)->applyPriv(thName+"+L",dbVarProp));
}

template<typename ValueType1, typename ValueType2>
ptr<typename SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::ATheoremType>
SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
applyFirstPriv(const std::string &thName, DbVarProp &dbVarProp) const
{
    return std::make_shared<const SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >>>(thName,
    get<0>(*m_son)->applyPriv(thName+"+R",dbVarProp),get<1>(*m_son)->applyPriv(thName+"+L",dbVarProp));
}

template<typename ValueType1, typename ValueType2>
typename SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::ValueType
SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::evaluate() const
{
    return m_son->evaluate();
}

template<typename ValueType1, typename ValueType2>
std::string SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<typename ValueType1, typename ValueType2>
const typename SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::SubOperatorType&
SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::getSon() const
{
    return *(m_son.get());
}

template<typename ValueType1, typename ValueType2>
const ptr<IISubRuleFormula>& SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::operator[]
(const size_t &k) const
{
    return (*m_son)[k];
}

}


#endif // PLUSRULEFORMULA_H
