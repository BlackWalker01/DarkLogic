/*===--- theorem.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements Theorem class in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "theorem.h"
#include "rule.h"
#include "Operator/operator.h"
#include "Formula/Arithmetic/allarithformula.h"
#include "Formula/Boolean/allproposition.h"
#include "logic.h"

using namespace N_DarkLogic;

/**---------------------------------------------------------------
 * Constructor methods from subProperties
 * ---------------------------------------------------------------
 */
Theorem<Hyp<ASubTheorem>>::Theorem(const std::vector<ptr<ASubTheorem>> &subProps):
    SubTheorem<SubPropertyType>(subProps)
{

}

Theorem<Not<ASubTheorem>>::Theorem(const ptr<ASubTheorem> &subProp):
    SubTheorem<SubPropertyType>(subProp)
{
    
}

Theorem<Boolean>::Theorem(const std::shared_ptr<Boolean>& son):
    SubTheorem<SubPropertyType>(son)
{
    
}

Theorem<ConstBoolean>::Theorem(const bool& val):
    SubTheorem<SubPropertyType>(val)
{
    
}


/**---------------------------------------------------------------
 * Copy constructors from SubTheorems
 * ---------------------------------------------------------------
 */
Theorem<ConstBoolean>::Theorem(const SubRule<ConstBoolean>& prop):
    SubTheorem<ConstBoolean>(prop)
{

}

/**---------------------------------------------------------------
 * ruleApply methods
 * ---------------------------------------------------------------
 */

ptr<IISubTheoremFormula> N_DarkLogic::Theorem<Hyp<ASubTheorem>>::ruleApply(const IISubRuleFormula &rule, DbVarProp& dbVarProp, 
    std::vector<Arity> &indexes, const Action::Id& actionKey) const
{
    Arity index=indexes[0];
    indexes.erase(indexes.begin());
    std::vector<ptr<ASubTheorem>> sons;
    for(size_t k=0;k<index;k++)
    {
        sons.push_back((*m_son)[k]);
    }
    sons.push_back(std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey,((*m_son)[index]),
        dbVarProp,indexes)));

    for(size_t k=index+1;k<m_son->arity();k++)
    {
        sons.push_back((*m_son)[k]);
    }

    return Logic::make_theorem_formula<Theorem<SubPropertyType>>(sons);
}

ptr<IISubTheoremFormula> N_DarkLogic::Theorem<Hyp<ASubTheorem>>::ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
    std::vector<Arity>& indexes, const Action::Id& actionKey, const size_t& logicIdx) const
{
    Arity index = indexes[0];
    indexes.erase(indexes.begin());
    std::vector<ptr<ASubTheorem>> sons;
    for (size_t k = 0; k < index; k++)
    {
        sons.push_back((*m_son)[k]);
    }
    sons.push_back(std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey, ((*m_son)[index]),
        dbVarProp, indexes, logicIdx)));

    for (size_t k = index + 1; k < m_son->arity(); k++)
    {
        sons.push_back((*m_son)[k]);
    }

    return Logic::make_theorem_formula<Theorem<SubPropertyType>>(logicIdx, sons);
}

ptr<IISubTheoremFormula> N_DarkLogic::Theorem<Not<ASubTheorem>>::ruleApply(const IISubRuleFormula &rule, DbVarProp& dbVarProp,
    std::vector<Arity> &indexes, const Action::Id& actionKey) const
{
    indexes.erase(indexes.begin());
    return Logic::make_theorem_formula<Theorem<Not<ASubTheorem>>>(
    std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey,(*m_son)[0],
        dbVarProp, indexes)));
}

ptr<IISubTheoremFormula> N_DarkLogic::Theorem<Not<ASubTheorem>>::ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
    std::vector<Arity>& indexes, const Action::Id& actionKey, const size_t& logicIdx) const
{
    indexes.erase(indexes.begin());
    return Logic::make_theorem_formula<Theorem<Not<ASubTheorem>>>(logicIdx,
        std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey, (*m_son)[0],
            dbVarProp, indexes, logicIdx)));
}

ptr<IISubTheoremFormula> N_DarkLogic::Theorem<Boolean>::ruleApply(const IISubRuleFormula &, DbVarProp&, std::vector<Arity> &, const Action::Id&) const
{
    throw std::runtime_error("Theorem Boolean connot call ruleApply method");
}

ptr<IISubTheoremFormula> N_DarkLogic::Theorem<Boolean>::ruleApply(const IISubRuleFormula&, DbVarProp&, std::vector<Arity>&, 
    const Action::Id&, const size_t&) const
{
    throw std::runtime_error("Theorem Boolean connot call ruleApply method");
}

ptr<IISubTheoremFormula> N_DarkLogic::Theorem<ConstBoolean>::ruleApply(const IISubRuleFormula &, DbVarProp&, std::vector<Arity> &,
                                                                       const Action::Id&) const
{
    throw std::runtime_error("Theorem ConstBoolean connot call ruleApply method");
}

ptr<IISubTheoremFormula> N_DarkLogic::Theorem<ConstBoolean>::ruleApply(const IISubRuleFormula&, DbVarProp&, std::vector<Arity>&,
    const Action::Id&, const size_t&) const
{
    throw std::runtime_error("Theorem ConstBoolean connot call ruleApply method");
}
