/*===--- rule.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines Rule class in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_RULE_H
#define DARK_LOGIC_RULE_H
#include "allrule.h"
#include "Formula/Arithmetic/inontermarithmetic.h"
#include "Utils/action.h"

namespace N_DarkLogic {

class DbVarProp;

template<SubRuleProperty SubPropertyType>
class Rule: public SubRule<SubPropertyType>
{
public:
    using ActionDb = std::unordered_map<Action::Id, ptr<ASubTheorem>>;
    using ActionDbPtr = std::shared_ptr<ActionDb>;
    using LastActionList = std::vector<ActionDbPtr>;


    Rule(const std::string& name_, const ptr<ASubRule>& leftSubProp, const ptr<ASubRule>& rightSubProp);

    std::string  name() const override final;

    //getAction methods
    std::vector<Action::Id> getActions(const ptr<ASubTheorem>& prop, Action::Id& lastActionIndex) const
    override final;
    std::vector<Action::Id> getActions(const ptr<ASubTheorem>& prop, Action::Id& lastActionIndex, const size_t& logicIdx) const
    override final;
    std::vector<Action> getHumanActions(const ptr<ASubTheorem>& prop, Action::Id& lastActionIndex) const override final;

    //inference methods
    std::pair<ptr<ASubTheorem>, bool> apply(const Action::Id& actionKey) const override final;
    ptr<ASubTheorem> apply(const Action::Id& actionKey, const ptr<ASubTheorem> &theorem, DbVarProp& dbVarProp, 
        std::vector<Arity>& indexes) const override final;
    ptr<ASubTheorem> apply(const Action::Id& actionKey, const ptr<ASubTheorem>& theorem, DbVarProp& dbVarProp, 
        std::vector<Arity>& indexes, const size_t& logicIdx) const
    override final;
    ptr<IISubTheoremFormula> applyAnnexe(const Action::Id& actionKey, const ptr<IISubTheoremFormula>& theorem, DbVarProp& dbVarProp, 
    std::vector<Arity>& indexes) const override final;
    ptr<IISubTheoremFormula> applyAnnexe(const Action::Id& actionKey, const ptr<IISubTheoremFormula>& theorem, DbVarProp& dbVarProp, std::vector<Arity>& indexes,
    const size_t& logicIdx) const override final;

    //reciprocity methods
    bool isSymetric() const override;
    ptr<Rule<Equivalent<ASubRule>>> getTrueEquivalent() const;
    std::vector<ptr<ASubRule>> getEquivalentRules() const override;

    //handle storage of actions
    void storeActions() const override final;
    void unstoreActions() const override final;

    ~Rule() override = default;

    void clearAllIdentifications() const override final;
private:    
    bool identify(const ptr<ASubTheorem>& prop, const std::vector<Arity>& path, DbVarProp& dbVarProp) const;
    void clearIdentifications() const;

    //intern attributes
    const std::string m_name;
    DbVarProp m_basicIdentifications;

    //extern attributes
    const std::unique_ptr<ActionDbPtr> m_crtActions;
    const std::unique_ptr<LastActionList> m_oldActions;
};

template<SubRuleProperty SubPropertyType> 
struct ToTheoremStruct<Rule<SubPropertyType>> { using Type = Theorem<SubPropertyType>; };

template<SubRuleProperty SubPropertyType>
Rule<SubPropertyType>::Rule(const std::string &name_, const ptr<ASubRule> &leftSubProp, const ptr<ASubRule> &rightSubProp):
    SubRule<SubPropertyType> (leftSubProp,rightSubProp), m_name(name_),
    m_basicIdentifications(this->getExtVars()->getVars()),
    m_crtActions(std::make_unique<ActionDbPtr>(std::make_shared<ActionDb>())),
    m_oldActions(std::make_unique<LastActionList>())
{

}

template<SubRuleProperty SubPropertyType>
inline std::string Rule<SubPropertyType>::name() const
{
    return m_name;
}

template<SubRuleProperty SubPropertyType>
std::vector<Action::Id> Rule<SubPropertyType>::getActions(const ptr<ASubTheorem> &prop, Action::Id &lastActionIndex) const
{
    std::vector<Action::Id> ret;

    //clear identifications associated with last property
    clearIdentifications();

    //get all possible paths from prop
    if constexpr (std::is_same_v<SubPropertyType, Equivalent<ASubRule>>)
    {
        for (const auto& path : prop->getAllPaths())
        {
            auto dbVarProp = m_basicIdentifications;
            auto crtPath = path;
            if (identify(prop, path, dbVarProp))
            {
                (**m_crtActions)[lastActionIndex] = apply(lastActionIndex, prop, dbVarProp, crtPath);
                ret.push_back(lastActionIndex);
                lastActionIndex++;
            }
        }
    }
    else
    {
        for (const auto& path : prop->getImplPaths())
        {
            auto dbVarProp = m_basicIdentifications;
            auto crtPath = path;
            if (identify(prop, path, dbVarProp))
            {
                (**m_crtActions)[lastActionIndex] = apply(lastActionIndex, prop, dbVarProp, crtPath);
                ret.push_back(lastActionIndex);
                lastActionIndex++;
            }
        }
    }

    return ret;
}

template<SubRuleProperty SubPropertyType>
inline std::vector<Action::Id> Rule<SubPropertyType>::getActions(const ptr<ASubTheorem>& prop, Action::Id& lastActionIndex, const size_t& logicIdx) const
{
    std::vector<Action::Id> ret;

    //clear identifications associated with last property
    clearIdentifications();

    //get all possible paths from prop
    if constexpr (std::is_same_v<SubPropertyType, Equivalent<ASubRule>>)
    {
        for (const auto& path : prop->getAllPaths())
        {
            auto dbVarProp = m_basicIdentifications;
            auto crtPath = path;
            if (identify(prop, path, dbVarProp))
            {
                (**m_crtActions)[lastActionIndex] = apply(lastActionIndex, prop, dbVarProp, crtPath, logicIdx);
                ret.push_back(lastActionIndex);
                lastActionIndex++;
            }
        }
    }
    else
    {
        for (const auto& path : prop->getImplPaths())
        {
            auto dbVarProp = m_basicIdentifications;
            auto crtPath = path;
            if (identify(prop, path, dbVarProp))
            {
                (**m_crtActions)[lastActionIndex] = apply(lastActionIndex, prop, dbVarProp, crtPath, logicIdx);
                ret.push_back(lastActionIndex);
                lastActionIndex++;
            }
        }
    }

    return ret;
}

template<SubRuleProperty SubPropertyType>
std::vector<Action> Rule<SubPropertyType>::getHumanActions(const ptr<ASubTheorem>& prop, Action::Id& lastActionIndex) const
{
    std::vector<Action> ret;

    //clear identifications associated with last property
    clearIdentifications();

    //get all possible paths from prop
    if constexpr (std::is_same_v<SubPropertyType, Equivalent<ASubRule>>)
    {
        for (const auto& path : prop->getAllPaths())
        {
            auto dbVarProp = m_basicIdentifications;
            auto crtPath = path;
            if (identify(prop, path, dbVarProp))
            {
                (**m_crtActions)[lastActionIndex] = apply(lastActionIndex, prop, dbVarProp, crtPath);
                ret.push_back(Action(lastActionIndex, this->m_name, this->toString(), path));
                lastActionIndex++;
            }
        }
    }
    else
    {
        for (const auto& path : prop->getImplPaths())
        {
            auto dbVarProp = m_basicIdentifications;
            auto crtPath = path;
            if (identify(prop, path, dbVarProp))
            {
                (**m_crtActions)[lastActionIndex] = apply(lastActionIndex, prop, dbVarProp, crtPath);
                ret.push_back(Action(lastActionIndex, this->m_name, this->toString(), path));
                lastActionIndex++;
            }
        }
    }

    return ret;
}

template<SubRuleProperty SubPropertyType>
inline std::pair<ptr<ASubTheorem>, bool> Rule<SubPropertyType>::apply(const Action::Id& actionKey) const
{
    return { (**m_crtActions)[actionKey], isSymetric() };
}

template<SubRuleProperty SubPropertyType>
ptr<ASubTheorem> Rule<SubPropertyType>::apply(const Action::Id& actionKey, const ptr<ASubTheorem>& theorem, DbVarProp& dbVarProp,
    std::vector<Arity>& indexes) const
{  
    //apply rule
    if(indexes.size()==0)
    {
        return (*(this->m_son))[0]->applyFirstPriv(dbVarProp);
    }
    else
    {
        return std::static_pointer_cast<const ASubTheorem>(applyAnnexe(actionKey, theorem, dbVarProp, indexes));
    }    
}

template<SubRuleProperty SubPropertyType>
inline ptr<ASubTheorem> Rule<SubPropertyType>::apply(const Action::Id& actionKey, const ptr<ASubTheorem>& theorem, DbVarProp& dbVarProp,
    std::vector<Arity>& indexes, const size_t& logicIdx) const
{
    //apply rule
    if (indexes.size() == 0)
    {
        return (*(this->m_son))[0]->applyFirstPriv(dbVarProp, logicIdx);
    }
    else
    {
        return std::static_pointer_cast<const ASubTheorem>(applyAnnexe(actionKey, theorem, dbVarProp, indexes, logicIdx));
    }
}

template<SubRuleProperty SubPropertyType>
ptr<IISubTheoremFormula> Rule<SubPropertyType>::applyAnnexe(const Action::Id& actionKey, const ptr<IISubTheoremFormula> &theorem, 
DbVarProp& dbVarProp, std::vector<Arity> &indexes) const
{
    if(indexes.size()==0)
    {
        return (*(this->m_son))[0]->applyPriv(dbVarProp);
    }
    else
    {
        return std::static_pointer_cast<const ASubTheorem>(theorem)->ruleApply(*this, dbVarProp, indexes, actionKey); //?? cast not only to ASubTheorem
    }
}

template<SubRuleProperty SubPropertyType>
inline ptr<IISubTheoremFormula> Rule<SubPropertyType>::applyAnnexe(const Action::Id& actionKey, const ptr<IISubTheoremFormula>& theorem, 
    DbVarProp& dbVarProp, std::vector<Arity>& indexes, const size_t& logicIdx) const
{
    if (indexes.size() == 0)
    {
        return (*(this->m_son))[0]->applyPriv(dbVarProp, logicIdx);
    }
    else
    {
        return std::static_pointer_cast<const ASubTheorem>(theorem)->ruleApply(*this, dbVarProp, indexes, 
            actionKey, logicIdx); //?? cast not only to ASubTheorem
    }
}

template<SubRuleProperty SubPropertyType>
std::vector<ptr<ASubRule>> Rule<SubPropertyType>::getEquivalentRules() const
{
    std::vector<ptr<ASubRule>> ret;
    std::vector<ptr<ASubRule>> leftRet = (*(this->m_son))[0]->getEquivalentRules();
    leftRet.push_back((*(this->m_son))[0]);
    std::vector<ptr<ASubRule>> rightRet = (*(this->m_son))[1]->getEquivalentRules();
    rightRet.push_back((*(this->m_son))[1]);

    size_t idx = 0;
    for (const auto& subRuleLeft : leftRet)
    {
        for (const auto& subRuleRight : rightRet)
        {
            if constexpr (std::is_same_v<SubPropertyType, Equivalent<ASubRule>>)
            {
                ret.push_back(std::make_shared<const Rule>(m_name+"_Annexe_"+sizeToString(idx),subRuleRight, subRuleLeft)); //symmetric case
                idx++;
            }
            ret.push_back(std::make_shared<const Rule>(m_name + "_Annexe_" + sizeToString(idx),subRuleLeft, subRuleRight)); //direct case
            idx++;
        }
    }

    //remove last one because it is the same as "this" SubRuleFormula
    ret.pop_back();
    return ret;
}

template<SubRuleProperty SubPropertyType>
inline bool Rule<SubPropertyType>::isSymetric() const
{
    return false;
}

template<SubRuleProperty SubPropertyType>
inline ptr<Rule<Equivalent<ASubRule>>> Rule<SubPropertyType>::getTrueEquivalent() const
{
    return std::make_shared<const Rule<Equivalent<ASubRule>>>(name() + "_True",
        std::make_shared<const SubRule<ConstBoolean>>(true),
        std::make_shared<const SubRule<SubPropertyType>>((*this)[0],(*this)[1]) 
        );
}

template<SubRuleProperty SubPropertyType>
inline void Rule<SubPropertyType>::storeActions() const
{
    //keep crt actions in memory
    m_oldActions->push_back(*m_crtActions);
}

template<SubRuleProperty SubPropertyType>
inline void Rule<SubPropertyType>::unstoreActions() const
{
    //get latest actions
    *m_crtActions = m_oldActions->back();
    m_oldActions->pop_back();
}

template<SubRuleProperty SubPropertyType>
inline void Rule<SubPropertyType>::clearAllIdentifications() const
{
    (*m_crtActions)->clear();
    m_oldActions->clear();
}

/**
 * @brief Rule::identify
 * @param prop
 * @param propIndexes
 * @return true if it can identify all variable in the antecedent of this rule with sub-properties of prop
 */
template<SubRuleProperty SubPropertyType>
bool Rule<SubPropertyType>::identify(const ptr<ASubTheorem> &prop, const std::vector<Arity>& path, DbVarProp& dbVarProp) const
{
    auto impl=(*(this->m_son))[1];
    ptr<IISubTheoremFormula> propToIdentify=prop;
    for(const size_t& index : path)
    {
        switch(propToIdentify->getFormulaType())
        {
            case IISubTheoremFormula::SUBPURETH:
            {
                propToIdentify=((*std::static_pointer_cast<const ASubPureTheorem>(propToIdentify))[index]);
                break;
            }
            case IISubTheoremFormula::SUBIMPURETH:
            {
                propToIdentify=((*std::static_pointer_cast<const ASubImpureTheorem>(propToIdentify))[index]);
                break;
            }
            case IISubTheoremFormula::NONTERMARITH:
            {
                propToIdentify=((*std::dynamic_pointer_cast<const INonTermArithmetic<IISubTheoremFormula>>(propToIdentify))[index]);
                break;
            }
            default:
            {
                throw std::runtime_error("Implementation error");
                break;
            }
        }
    }
    if(impl->identifyPriv(std::static_pointer_cast<const ASubTheorem>(propToIdentify),dbVarProp))
    {
        return dbVarProp.isTotallyIdentified();
    }
    else
    {
        return false;
    }
}

template<SubRuleProperty SubPropertyType>
void Rule<SubPropertyType>::clearIdentifications() const
{
    (*m_crtActions) = std::make_shared<ActionDb>();
}

}

#endif // DARK_LOGIC_RULE_H
