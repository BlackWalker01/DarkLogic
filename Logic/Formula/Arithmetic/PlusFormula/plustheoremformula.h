#ifndef PLUSTHEOREMFORMULA_H
#define PLUSTHEOREMFORMULA_H
#include "Formula/Arithmetic/subarithmetictheorem.h"
#include "Formula/Arithmetic/inontermarithmetic.h"

namespace N_Logic
{

template<typename ValueType1, typename ValueType2>
class SubArithmeticTheorem<Plus< ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>>:
        public ASubArithmeticTheorem<typename Plus< ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>::ValueType>,
        public INonTermArithmetic<IISubTheoremFormula>
{
public:
    typedef ASubArithmeticTheorem<ValueType1> SubLeftFormula;
    typedef ASubArithmeticTheorem<ValueType2> SubRightFormula;
    typedef Plus< SubLeftFormula,SubRightFormula> SubOperatorType;
    typedef typename SubOperatorType::ValueType ValueType;

    SubArithmeticTheorem(const std::string& name_, const ptr<SubLeftFormula>& leftFormula,
                         const ptr<SubRightFormula>& rightFormula);

    size_t arity() const override final;
    ValueType evaluate() const override final;

    const SubOperatorType& getSon() const;
    const ptr<IISubTheoremFormula>& operator[](const size_t& k) const override final;
    IISubTheoremFormula::TheoremFormulaType getFormulaType() const override final;
    std::string toString(unsigned short priorityParent=1000) const override final;
    const std::vector<std::vector<Arity>>& computeAllPaths() override final;

    bool isEqual(const ASubArithmeticRule<ValueType>& prop) const override final;
    bool isEqual(const ASubArithmeticTheorem<ValueType>& prop) const override final;
    bool operator==(const SubArithmeticTheorem& prop) const;
    bool operator==(const SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>>& prop) const;

    ~SubArithmeticTheorem() override = default;
private:
    const std::unique_ptr<const SubOperatorType> m_son;
};

template<typename ValueType1, typename ValueType2>
SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
SubArithmeticTheorem(const std::string &name_,
                     const ptr<ASubArithmeticTheorem<ValueType1>>& leftFormula,
                     const ptr<ASubArithmeticTheorem<ValueType2>>& rightFormula):
    ASubArithmeticTheorem<typename Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >::ValueType>(name_,PLUS_FORMULA),
m_son(std::make_unique<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2>> >(leftFormula,rightFormula))
{

}

template<typename ValueType1, typename ValueType2>
size_t SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::arity() const
{
    return m_son->arity();
}

template<typename ValueType1, typename ValueType2>
typename SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::ValueType
SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::evaluate() const
{
    return m_son->evaluate();
}

template<typename ValueType1, typename ValueType2>
const typename SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::SubOperatorType&
SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::getSon() const
{
    return *(m_son.get());
}

template<typename ValueType1, typename ValueType2>
std::string SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<typename ValueType1, typename ValueType2>
bool SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
isEqual(const ASubArithmeticRule<ValueType> &prop) const
{
    auto propCast=
            dynamic_cast<const SubArithmeticRule<Plus< ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2> >>*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<typename ValueType1, typename ValueType2>
bool SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
isEqual(const ASubArithmeticTheorem<ValueType> &prop) const
{
    auto propCast=dynamic_cast<const SubArithmeticTheorem*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<typename ValueType1, typename ValueType2>
bool SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
operator==(const SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > > &prop) const
{
    return *m_son==(prop.getSon());
}

template<typename ValueType1, typename ValueType2>
bool SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
operator==(const SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>> &prop) const
{
    return *m_son==(prop.getSon());
}

template<typename ValueType1, typename ValueType2>
const std::vector<std::vector<Arity> >&
SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::computeAllPaths()
{
    if(!this->m_allPaths.size())
    {
        this->m_allPaths.push_back({});

        //left subProperty
        std::vector<std::vector<Arity>> leftPath=get<0>(*m_son)->getAllPaths();
        for(size_t k=0; k<leftPath.size();k++)
        {
            std::vector<Arity> crtPath=leftPath[k];
            crtPath.insert(crtPath.begin(),0);
            this->m_allPaths.push_back(crtPath);
        }

        //right subProperty
        std::vector<std::vector<Arity>> rightPath=get<1>(*m_son)->getAllPaths();
        for(size_t k=0; k<rightPath.size();k++)
        {
            std::vector<Arity> crtPath=rightPath[k];
            crtPath.insert(crtPath.begin(),1);
            this->m_allPaths.push_back(crtPath);
        }
    }
    return this->m_allPaths;
}

template<typename ValueType1, typename ValueType2>
IISubTheoremFormula::TheoremFormulaType N_Logic::SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>,
ASubArithmeticTheorem<ValueType2> > >::getFormulaType() const
{
    return IISubTheoremFormula::TheoremFormulaType::NONTERMARITH;
}

template<typename ValueType1, typename ValueType2>
const ptr<IISubTheoremFormula> &SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
operator[](const size_t &k) const
{
    return (*m_son)[k];
}

}

#endif // PLUSTHEOREMFORMULA_H
