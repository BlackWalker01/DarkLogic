/*===--- subtheoremgeneric.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubTheorem generic case in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SUBTHEOREMGENERIC_H
#define DARK_LOGIC_SUBTHEOREMGENERIC_H
#include "Formula/Boolean/subtheorem.h"
#include "Formula/Boolean/asubpuretheorem.h"

namespace N_DarkLogic
{
template<SubTheoremProperty SubPropertyType>
class SubTheorem: public ASubPureTheorem
{
public:
    SubTheorem(const ptr<ASubTheorem>& leftSubProp, const ptr<ASubTheorem>& rightSubProp);    

    bool evaluate() const override;
    constexpr PropType type() const override final
    {
        if constexpr (std::is_same_v<SubPropertyType, And<ASubTheorem>>)
        {
            return AND_PROP;
        }
        else if constexpr (std::is_same_v<SubPropertyType, Equivalent<ASubTheorem>>)
        {
            return EQUIV_PROP;
        }
        else if constexpr (std::is_same_v<SubPropertyType, Implication<ASubTheorem>>)
        {
            return IMPL_PROP;
        }
        else if constexpr (std::is_same_v<SubPropertyType, Not<ASubTheorem>>)
        {
            return NOT_PROP;
        }
        else if constexpr (std::is_same_v<SubPropertyType, Or<ASubTheorem>>)
        {
            return OR_PROP;
        }
    }

    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubTheorem& prop) const;
    bool isEqual(const ASubRule& prop) const override final;
    bool operator==(const SubRule<ToRuleOpe<SubPropertyType>>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;

    size_t arity() const override final;
    ptr<ASubTheorem> copyTheorem() const override final;
    const SubPropertyType& getSon() const;
    const DbVar* getExtVars() const override final;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, std::vector<size_t>& indexes, const size_t& actionKey) const override;

    ~SubTheorem() override = default;
protected:
    const std::vector<std::vector<Arity> > &computeAllPaths() override final;
    const std::vector<std::vector<Arity>>& computeImplPaths() override final;

protected:
    const ptr<ASubTheorem>& operator[](const size_t& index) const override final;

    const std::unique_ptr<const SubPropertyType> m_son;
    const DbVar m_extVars;
};

template<SubTheoremProperty SubPropertyType>
const SubPropertyType &SubTheorem<SubPropertyType>::getSon() const
{
    return *(m_son.get());
}

template<SubTheoremProperty SubPropertyType>
const DbVar* SubTheorem<SubPropertyType>::getExtVars() const
{
    return &m_extVars;
}

template<SubTheoremProperty SubPropertyType>
const std::vector<std::vector<Arity> >& SubTheorem<SubPropertyType>::computeAllPaths()
{
    if(!m_allPaths.size())
    {
        m_allPaths.push_back({});

        //left subProperty
        std::vector<std::vector<Arity>> leftPath=(*m_son)[0]->getAllPaths();
        for(size_t k=0; k<leftPath.size();k++)
        {
            std::vector<Arity> crtPath=leftPath[k];
            crtPath.insert(crtPath.begin(),0);
            m_allPaths.push_back(crtPath);
        }

        //right subProperty
        std::vector<std::vector<Arity>> rightPath=(*m_son)[1]->getAllPaths();
        for(size_t k=0; k<rightPath.size();k++)
        {
            std::vector<Arity> crtPath=rightPath[k];
            crtPath.insert(crtPath.begin(),1);
            m_allPaths.push_back(crtPath);
        }
    }
    return m_allPaths;
}

/**
 * type methods
 */

template<SubTheoremProperty SubPropertyType> struct ToRuleStruct<SubTheorem<SubPropertyType>> { using Type = SubRule<SubPropertyType>; };

}
#endif // DARK_LOGIC_SUBTHEOREMGENERIC_H
