/*===--- subtheoremlet.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubTheorem<Let<SubTheoremType,ASubTheorem>> in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SUBTHEOREMLET_H
#define DARK_LOGIC_SUBTHEOREMLET_H
#include "Formula/Boolean/subtheorem.h"
#include "Formula/Boolean/asubimpuretheorem.h"
#include "Set/allset.h"

namespace N_DarkLogic
{
template<typename SubTheoremType>
class SubTheorem<Let<SubTheoremType,ASubTheorem>>: public ASubImpureTheorem
{
public:
    typedef Let<SubTheoremType,ASubTheorem> SubPropertyType;

    SubTheorem(const ptr<SubTheoremType>& leftFormula,
               const ptr<ASubTheorem>& rightFormula);
    SubTheorem(const ptr<SubRule<ConstBoolean>>& prop);

    bool evaluate() const override;
    const State& getState() const override final;
    constexpr PropType type() const override final;

    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubTheorem& prop) const;
    bool isEqual(const ASubRule& prop) const override final;
    template<typename SubRuleType>
    bool operator==(const SubRule<Let<SubRuleType,ASubRule>>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;
    const std::vector<std::vector<Arity> > &computeAllPaths() override final;
    const std::vector<std::vector<Arity>>& computeImplPaths() override final;

    ptr<ASubTheorem> copyTheorem() const override final;
    const SubPropertyType& getSon() const;
    const DbVar* getExtVars() const override final;
    const ptr<IISubTheoremFormula>& operator[](const size_t& index) const override final;

    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
        std::vector<Arity>& indexes, const Action::Id& actionKey) const override;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
        std::vector<Arity>& indexes, const Action::Id& actionKey, const size_t& logicIdx) const override;

    ~SubTheorem() override = default;

protected:
    size_t arity() const override final;

    const std::unique_ptr<const SubPropertyType> m_son;
    const DbVar m_extVars;
    const State m_state;
};

/*template<typename ValueType1, typename ValueType2>
SubTheorem<Let<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::SubTheorem(const SubRule<ConstBoolean> &prop)
{

}*/

template<typename SubTheoremType>
SubTheorem<Let<SubTheoremType, ASubTheorem> >::
SubTheorem(const ptr<SubTheoremType>& leftFormula, const ptr<ASubTheorem>& rightFormula):
    m_son(std::make_unique<Let<SubTheoremType, ASubTheorem>>(leftFormula,rightFormula)),
    m_extVars(leftFormula->getExtVars(), rightFormula->getExtVars()),
    m_state(LET, leftFormula->getState(), rightFormula->getState())
{
    computeAllPaths();
    computeImplPaths();
}

template<typename SubTheoremType>
const typename SubTheorem<Let<SubTheoremType, ASubTheorem > >::SubPropertyType&
SubTheorem<Let<SubTheoremType, ASubTheorem> >::getSon() const
{
    return *m_son;
}

template<typename SubTheoremType>
constexpr IProposition::PropType SubTheorem < Let<SubTheoremType, ASubTheorem>>::type() const
{
    return IProposition::LET_PROP;
}

template<typename SubTheoremType>
size_t SubTheorem<Let<SubTheoremType, ASubTheorem> >::arity() const
{
    return m_son->arity();
}

template<typename SubTheoremType>
bool SubTheorem<Let<SubTheoremType, ASubTheorem> >::evaluate() const
{
    return m_son->evaluate();
}

template<typename SubTheoremType>
const State& SubTheorem<Let<SubTheoremType, ASubTheorem> >::getState() const
{
    return m_state;
}

template<typename SubTheoremType>
std::string SubTheorem<Let<SubTheoremType, ASubTheorem> >::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<typename SubTheoremType>
inline const DbVar* SubTheorem<Let<SubTheoremType, ASubTheorem>>::getExtVars() const
{
    return &m_extVars;
}

template<typename SubTheoremType>
bool SubTheorem<Let<SubTheoremType, ASubTheorem> >::isEqual(const ASubTheorem &prop) const
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

template<typename SubTheoremType>
bool SubTheorem<Let<SubTheoremType, ASubTheorem> >::
operator==(const SubTheorem<Let<SubTheoremType, ASubTheorem > > &prop) const
{
    return *m_son==prop.getSon();
}
/*
template<typename SubTheoremType>
bool SubTheorem<Let<SubTheoremType, ASubTheorem> >::operator==(ASubRule &prop) const
{
    //FIX IT!!!!!!!!! not SubPropertyType
    SubRule<SubPropertyType>* propCast=dynamic_cast<SubRule<SubPropertyType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==*(propCast->getSon());
    }
    else
    {
        return false;
    }
}*/

template<typename SubTheoremType> template<typename SubRuleType>
bool SubTheorem<Let<SubTheoremType, ASubTheorem> >::
operator==(const SubRule<Let<SubRuleType,ASubRule>> &prop) const
{
    return *m_son==prop.getSon();
}


template<typename SubTheoremType>
ptr<IISubTheoremFormula> SubTheorem<Let<SubTheoremType, ASubTheorem > >::
ruleApply(const IISubRuleFormula &/*rule*/, DbVarProp& /*dbVarProp*/, std::vector<Arity> &indexes, const Action::Id&/*actionKey*/) const
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

template<typename SubTheoremType>
ptr<IISubTheoremFormula> SubTheorem<Let<SubTheoremType, ASubTheorem > >::
ruleApply(const IISubRuleFormula&/*rule*/, DbVarProp& /*dbVarProp*/, std::vector<Arity>& indexes, const Action::Id&/*actionKey*/, 
    const size_t& /*logicIdx*/) const
{
    Arity index = indexes[0];
    indexes.erase(indexes.begin());
    if (index == 0)
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

template<typename SubTheoremType>
const std::vector<std::vector<Arity> > &SubTheorem<Let<SubTheoremType, ASubTheorem> >::
computeAllPaths()
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

template<typename SubTheoremType>
const std::vector<std::vector<Arity> >& SubTheorem<Let<SubTheoremType, ASubTheorem> >::
computeImplPaths()
{
    if (!m_implPaths.size())
    {
        m_implPaths.push_back({});

        //left subProperty
        std::vector<std::vector<Arity>> leftPath = get<0>(*m_son)->getImplPaths();
        for (size_t k = 0; k < leftPath.size(); k++)
        {
            std::vector<Arity> crtPath = leftPath[k];
            crtPath.insert(crtPath.begin(), 0);
            m_implPaths.push_back(crtPath);
        }

        //right subProperty
        std::vector<std::vector<Arity>> rightPath = get<1>(*m_son)->getImplPaths();
        for (size_t k = 0; k < rightPath.size(); k++)
        {
            std::vector<Arity> crtPath = rightPath[k];
            crtPath.insert(crtPath.begin(), 1);
            m_implPaths.push_back(crtPath);
        }
    }
    return m_implPaths;
}

template<typename SubTheoremType>
const ptr<IISubTheoremFormula> &N_DarkLogic::SubTheorem<Let<SubTheoremType, ASubTheorem> >::operator[](const size_t &index) const
{
    return (*m_son)[index];
}

}
#endif // DARK_LOGIC_SUBTHEOREMLET_H
