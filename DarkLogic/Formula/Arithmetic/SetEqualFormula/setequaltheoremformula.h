/*===--- setequaltheoremformula.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubArithmeticTheorem in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SETEQUALTHEOREMFORMULA_H
#define DARK_LOGIC_SETEQUALTHEOREMFORMULA_H
#include "Formula/Arithmetic/subarithmetictheorem.h"
#include "Formula/Arithmetic/inontermarithmetic.h"

namespace N_DarkLogic
{

template<typename VariableType_>
class SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType_>>>:
        public ASubArithmeticTheorem<void>,
        public INonTermArithmetic<IISubTheoremFormula>
{
public:
    typedef SubArithmeticTheorem<VariableType_> SubLeftFormula;
    typedef ASubArithmeticTheorem<typename VariableType_::ValueType> SubRightFormula;
    typedef SetEqual< SubLeftFormula> SubOperatorType;
    using SubPropertyType = SubOperatorType;
    typedef typename SubOperatorType::ValueType ValueType;

    SubArithmeticTheorem(const ptr<SubLeftFormula>& leftFormula,
                         const ptr<SubRightFormula>& rightFormula);

    size_t arity() const override final;
    ValueType evaluate() const override final;
    const State& getState() const override final;
    constexpr ArithType type() const override final;

    const SubOperatorType& getSon() const;
    const ptr<IISubTheoremFormula>& operator[](const size_t& k) const override final;
    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;
    const std::vector<std::vector<Arity>>& computeAllPaths() override final;
    const std::vector<std::vector<Arity>>& computeImplPaths() override final;
    TheoremFormulaType getFormulaType() const override final;

    bool isEqual(const ASubArithmeticRule<ValueType>& prop) const override final;
    bool isEqual(const ASubArithmeticTheorem<ValueType>& prop) const override final;
    bool operator==(const SubArithmeticTheorem& prop) const;
    bool operator==(const SubArithmeticRule<ToRuleOpe<SubOperatorType>>& prop) const;

    ~SubArithmeticTheorem() override = default;
private:
    const std::unique_ptr<const SubOperatorType> m_son;
    const DbVar m_extVars;
    const State m_state;
};

template<typename VariableType>
SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::
SubArithmeticTheorem(const ptr<SubArithmeticTheorem<VariableType>>& leftFormula,
                     const ptr<ASubArithmeticTheorem<typename VariableType::ValueType>>& rightFormula):
m_son(std::make_unique<SetEqual<SubArithmeticTheorem<VariableType>>>(leftFormula,rightFormula)),
m_extVars(leftFormula->getExtVars(), rightFormula->getExtVars()),
m_state(SETEQUAL, leftFormula->getState(), rightFormula->getState())
{
    computeAllPaths();
    computeImplPaths();
}

template<typename VariableType>
constexpr ArithType SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::type() const
{
    return SET_FORMULA;
}

template<typename VariableType>
size_t SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::arity() const
{
    return m_son->arity();
}

template<typename VariableType>
typename SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::ValueType
SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::evaluate() const
{
    m_son->evaluate();
}

template<typename VariableType>
const State&
SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::getState() const
{
    return m_state;
}

template<typename VariableType>
const typename SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::SubOperatorType&
SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::getSon() const
{
    return *(m_son.get());
}

template<typename VariableType>
std::string SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::
toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<typename VariableType>
const DbVar* SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::getExtVars() const
{
    return &m_extVars;
}

template<typename VariableType>
bool SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::
isEqual(const ASubArithmeticRule<ValueType> &prop) const
{
    auto propCast=
            dynamic_cast<const SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>>>*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<typename VariableType>
bool SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::
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

template<typename VariableType>
bool SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::
operator==(const SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> > &prop) const
{
    return *m_son==(prop.getSon());
}

template<typename VariableType>
bool SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::
operator==(const SubArithmeticRule<ToRuleOpe<SetEqual<SubArithmeticTheorem<VariableType>>> > &prop) const
{
    return *m_son==(prop.getSon());
}


template<typename VariableType>
const std::vector<std::vector<Arity> > &
SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::computeAllPaths()
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


template<typename VariableType>
const std::vector<std::vector<Arity> >&
SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >::computeImplPaths()
{
    if (!this->m_implPaths.size())
    {
        this->m_implPaths.push_back({});

        //left subProperty
        std::vector<std::vector<Arity>> leftPath = get<0>(*m_son)->getImplPaths();
        for (size_t k = 0; k < leftPath.size(); k++)
        {
            std::vector<Arity> crtPath = leftPath[k];
            crtPath.insert(crtPath.begin(), 0);
            this->m_implPaths.push_back(crtPath);
        }

        //right subProperty
        std::vector<std::vector<Arity>> rightPath = get<1>(*m_son)->getImplPaths();
        for (size_t k = 0; k < rightPath.size(); k++)
        {
            std::vector<Arity> crtPath = rightPath[k];
            crtPath.insert(crtPath.begin(), 1);
            this->m_implPaths.push_back(crtPath);
        }
    }
    return this->m_implPaths;
}

template<typename VariableType_>
IISubTheoremFormula::TheoremFormulaType SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType_> > >::getFormulaType() const
{
    return NONTERMARITH;
}

template<typename VariableType_>
const ptr<IISubTheoremFormula> &N_DarkLogic::SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType_> > >::
operator[](const size_t &k) const
{
    return (*m_son)[k];
}

}

#endif // DARK_LOGIC_SETEQUALTHEOREMFORMULA_H
