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
template<SubTheoremProperty SubPropertyType_>
class SubTheorem: public ASubPureTheorem
{
public:
    using SubPropertyType = SubPropertyType_;
    SubTheorem(const ptr<ASubTheorem>& leftSubProp, const ptr<ASubTheorem>& rightSubProp);    

    //evaluation methods
    bool isEvaluated() const override final;
    bool evaluate() const override final;
    bool canBeDemonstrated() const override final;
    bool testEvaluate(const Evaluater::ConfigEval& configEval) const override final;
    bool getHiddenValue() const override final;
    std::unordered_map<IDVar, IDVar> getVarToEval() const override final;
    std::vector<std::pair<Evaluater::ConfigEval, bool>> getConfigEvals() const override final;
    std::vector<Evaluater::ConfigEval> getCompatibleConfigs(const Evaluater::ConfigEval& commonConfig,
        const std::unordered_map<IDVar, IDVar>& internalVars) const override final;
    
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
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
        std::vector<Arity>& indexes, const Action::Id& actionKey) const override;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
        std::vector<Arity>& indexes, const Action::Id& actionKey, const size_t& logicIdx) const override;

    ~SubTheorem() override = default;
protected:
    const std::vector<std::vector<Arity> > &computeAllPaths() override final;
    const std::vector<std::vector<Arity>>& computeImplPaths() override final;

private:
    void initEval();

protected:
    const ptr<ASubTheorem>& operator[](const size_t& index) const override final;

    const std::unique_ptr<const SubPropertyType> m_son;
    const DbVar m_extVars;
    const std::unique_ptr<Evaluater> m_eval;
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

template<SubTheoremProperty SubPropertyType>
bool SubTheorem<SubPropertyType>::testEvaluate(const Evaluater::ConfigEval& configEval) const
{
    return m_eval->evalWithConfig(configEval);
}

template<SubTheoremProperty SubPropertyType>
bool N_DarkLogic::SubTheorem<SubPropertyType>::isEvaluated() const
{
    return m_eval->isEvaluated();
}

template<SubTheoremProperty SubPropertyType>
std::unordered_map<IDVar, IDVar> SubTheorem<SubPropertyType>::getVarToEval() const
{
    return m_eval->getVarToEval();
}

template<SubTheoremProperty SubPropertyType>
inline std::vector<Evaluater::ConfigEval> SubTheorem<SubPropertyType>::getCompatibleConfigs(const Evaluater::ConfigEval& commonConfig, 
    const std::unordered_map<IDVar, IDVar>& internalVars) const
{
    return m_eval->getCompatibleConfigs(commonConfig,internalVars);
}

/**
 * type methods
 */

template<SubTheoremProperty SubPropertyType> struct ToRuleStruct<SubTheorem<SubPropertyType>> { using Type = SubRule<SubPropertyType>; };

}
#endif // DARK_LOGIC_SUBTHEOREMGENERIC_H
