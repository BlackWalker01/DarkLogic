/*===--- plusruleformula.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubArithmeticRule in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_PLUSRULEFORMULA_H
#define DARK_LOGIC_PLUSRULEFORMULA_H
#include "Formula/Arithmetic/subarithmeticrule.h"
#include "Operator/Arithmetic/plus.h"
#include "Formula/Arithmetic/inontermarithmetic.h"
#include "logic.h"

namespace N_DarkLogic
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
    using SubPropertyType = SubOperatorType;
    typedef typename SubOperatorType::ValueType ValueType;
    typedef typename
    ASubArithmeticRule<typename Plus< ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>::ValueType>::ATheoremType ATheoremType;
    using ARuleType = ASubArithmeticRule<ValueType>;

    SubArithmeticRule(const ptr<SubLeftFormula>& leftFormula, const ptr<SubRightFormula>& rightFormula);

    size_t arity() const override final;
    ValueType evaluate() const override final;
    const State& getState() const override final;
    constexpr ArithType type() const override final;

    //internal methods
    bool identifyPriv(const ptr<ATheoremType>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const override final;
    ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const override final;

    bool isEqual(const ASubArithmeticRule<ValueType>& prop) const override final;
    bool isEqual(const ASubArithmeticTheorem<ValueType>& prop) const override final;
    bool operator==(const SubArithmeticRule& prop) const;
    bool operator==(const SubArithmeticTheorem<ToTheoremOpe<SubOperatorType>>& prop) const;

    std::vector<ptr<ARuleType>> getEquivalentRules() const override final;
    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;

    const SubOperatorType& getSon() const;
    const ptr<IISubRuleFormula>& operator[](const size_t& k) const override final;

    ~SubArithmeticRule() override = default;

protected:
    const std::unique_ptr<const SubOperatorType> m_son;
    const DbVar m_extVars;
    const State m_state;
};

template<typename ValueType1, typename ValueType2>
SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
SubArithmeticRule(const ptr<SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::SubLeftFormula> &leftFormula,
                  const ptr<SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::SubRightFormula> &rightFormula):
    m_son(std::make_unique<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2>> >(leftFormula,rightFormula)),
    m_extVars(leftFormula->getExtVars(), rightFormula->getExtVars()),
    m_state(PLUS, leftFormula->getState(), rightFormula->getState())
{

}


template<typename ValueType1, typename ValueType2>
size_t SubArithmeticRule< Plus< ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2> > >::arity() const
{
    return m_son->arity();
}

template<typename ValueType1, typename ValueType2>
inline const State& SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2>>>::getState() const
{
    return m_state;
}

template<typename ValueType1, typename ValueType2>
inline constexpr ArithType SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2>>>::type() const
{
    return PLUS_FORMULA;
}


template<typename ValueType1, typename ValueType2>
bool SubArithmeticRule< Plus< ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2> > >::
isEqual(const ASubArithmeticRule<ValueType>& prop) const
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
isEqual(const ASubArithmeticTheorem<ValueType>& prop) const
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
(const SubArithmeticTheorem<ToTheoremOpe<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2>> >> &prop) const
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
applyPriv(DbVarProp &dbVarProp) const
{
    return Logic::make_theorem_formula<SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >>>(
    get<0>(*m_son)->applyPriv(dbVarProp),get<1>(*m_son)->applyPriv(dbVarProp));
}

template<typename ValueType1, typename ValueType2>
ptr<typename SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::ATheoremType>
SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
applyFirstPriv(DbVarProp &dbVarProp) const
{
    return Logic::make_theorem_formula<SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >>>(
    get<0>(*m_son)->applyPriv(dbVarProp),get<1>(*m_son)->applyPriv(dbVarProp));
}

template<typename ValueType1, typename ValueType2>
ptr<typename SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::ATheoremType>
SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >>>(
        get<0>(*m_son)->applyPriv(dbVarProp, logicIdx), get<1>(*m_son)->applyPriv(dbVarProp, logicIdx));
}

template<typename ValueType1, typename ValueType2>
ptr<typename SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::ATheoremType>
SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<SubArithmeticTheorem<Plus<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >>>(
        get<0>(*m_son)->applyPriv(dbVarProp, logicIdx), get<1>(*m_son)->applyPriv(dbVarProp, logicIdx));
}

template<typename ValueType1, typename ValueType2>
typename SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::ValueType
SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::evaluate() const
{
    return m_son->evaluate();
}

template<typename ValueType1, typename ValueType2>
inline std::vector<ptr<typename SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2>>>::ARuleType>>
SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2>>>::getEquivalentRules() const
{
    std::vector<ptr<ARuleType>> ret;
    std::vector<ptr<ASubArithmeticRule<ValueType1>>> leftRet = get<0>(*m_son)->getEquivalentRules();
    leftRet.push_back(get<0>(*m_son));
    std::vector<ptr<ASubArithmeticRule<ValueType2>>> rightRet = get<1>(*m_son)->getEquivalentRules();
    rightRet.push_back(get<1>(*m_son));
    
    for (const auto& subRuleLeft : leftRet)
    {
        for (const auto& subRuleRight : rightRet)
        {            
            ret.push_back(std::make_shared<const SubArithmeticRule<Plus<ASubArithmeticRule<ValueType2>, 
                ASubArithmeticRule<ValueType1>>>>(subRuleRight, subRuleLeft)); //symmetric case
            ret.push_back(std::make_shared<const SubArithmeticRule>(subRuleLeft, subRuleRight)); //direct case
        }        
    }

    //remove last one because it is the same as "this" SubRuleFormula
    ret.pop_back();

    return ret;
}

template<typename ValueType1, typename ValueType2>
std::string SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<typename ValueType1, typename ValueType2>
inline const DbVar* SubArithmeticRule<Plus<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2>>>::getExtVars() const
{
    return &m_extVars;
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


#endif // DARK_LOGIC_PLUSRULEFORMULA_H
