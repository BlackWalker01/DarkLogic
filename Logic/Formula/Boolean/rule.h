#ifndef RULE_H
#define RULE_H
#include "allrule.h"
#include "Formula/Arithmetic/inontermarithmetic.h"
#include "Utils/action.h"

namespace N_Logic {

ptr<ASubRule> createRule(const std::string& name, const std::string& content);
ptr<ASubRule> createRule(const std::string &name, std::vector<OperatorOrdering> &opeList,
                            std::vector<OperatorOrdering> &orderedOpeList,
                             std::vector<std::shared_ptr<VariableContainer> > &varList);

class DbVarProp;

template<SubRuleProperty SubPropertyType>
class Rule: public SubRule<SubPropertyType>
{
public:
    Rule(const std::string& name_, const ptr<ASubRule>& leftSubProp, const ptr<ASubRule>& rightSubProp);

    std::string  name() const override final;

    //getAction methods
    std::vector<size_t> getActions(const ptr<ASubTheorem>& prop, size_t& lastActionIndex) const
    override final;
    std::vector<Action> getHumanActions() const override final;

    //inference methods
    std::pair<ptr<ASubTheorem>,bool> apply(const size_t& actionKey, const ptr<ASubTheorem> &theorem) const override final;
    ptr<IISubTheoremFormula> applyAnnexe(const size_t& actionKey, const ptr<IISubTheoremFormula>& theorem, std::vector<Arity>& indexes) const override final;
    void unapply() const override final;

    //reciprocity methods
    bool isSymetric() const override;
    ptr<Rule> getReciprocal() const;
    ptr<Rule> getTrueEquivalent() const;

    ~Rule() override = default;

private:    
    bool identify(const ptr<ASubTheorem>& prop, const std::vector<Arity>& path, DbVarProp& dbVarProp) const;
    void clearIdentifications() const;

    const std::string m_name;
    DbVarProp m_basicIdentifications;
    const std::unique_ptr<std::unordered_map<size_t,std::shared_ptr<DbVarProp>>> m_actionToIdentifications;
    const std::unique_ptr<std::unordered_map<size_t,std::vector<Arity>>> m_crtActions;
};

template<SubRuleProperty SubPropertyType> 
struct ToTheoremStruct<Rule<SubPropertyType>> { using Type = Theorem<SubPropertyType>; };

template<SubRuleProperty SubPropertyType>
Rule<SubPropertyType>::Rule(const std::string &name_, const ptr<ASubRule> &leftSubProp, const ptr<ASubRule> &rightSubProp):
    SubRule<SubPropertyType> (leftSubProp,rightSubProp), m_name(name_),
    m_basicIdentifications(this->getExtVars()->getVars()),
    m_actionToIdentifications(std::make_unique<std::unordered_map<size_t,std::shared_ptr<DbVarProp>>>()),
    m_crtActions(std::make_unique<std::unordered_map<size_t,std::vector<Arity>>>())
{

}

template<SubRuleProperty SubPropertyType>
inline std::string Rule<SubPropertyType>::name() const
{
    return m_name;
}

template<SubRuleProperty SubPropertyType>
std::vector<size_t> Rule<SubPropertyType>::getActions(const ptr<ASubTheorem> &prop, size_t &lastActionIndex) const
{
    std::vector<size_t> ret;

    //clear identifications associated with last property
    clearIdentifications();

    //get all possible paths from prop
    if constexpr (std::is_same_v<SubPropertyType, Equivalent<ASubRule>>)
    {
        for (const auto& path : prop->getAllPaths())
        {
            auto dbVarProp = std::make_shared<DbVarProp>(m_basicIdentifications);
            if (identify(prop, path, *dbVarProp))
            {
                (*m_crtActions)[lastActionIndex] = path;
                (*m_actionToIdentifications)[lastActionIndex] = dbVarProp;
                ret.push_back(lastActionIndex);
                lastActionIndex++;
            }
        }
    }
    else
    {
        for (const auto& path : prop->getImplPaths())
        {
            auto dbVarProp = std::make_shared<DbVarProp>(m_basicIdentifications);
            if (identify(prop, path, *dbVarProp))
            {
                (*m_crtActions)[lastActionIndex] = path;
                (*m_actionToIdentifications)[lastActionIndex] = dbVarProp;
                ret.push_back(lastActionIndex);
                lastActionIndex++;
            }
        }
    }

    return ret;
}

template<SubRuleProperty SubPropertyType>
std::vector<Action> Rule<SubPropertyType>::getHumanActions() const
{
    std::vector<Action> ret;
    for(auto it=m_crtActions->begin();it!=m_crtActions->end();it++)
    {
        ret.push_back(Action(it->first,this->m_name,this->toString(),it->second));
    }
    return ret;
}

template<SubRuleProperty SubPropertyType>
std::pair<ptr<ASubTheorem>,bool> Rule<SubPropertyType>::apply(const size_t &actionKey, const ptr<ASubTheorem> &theorem) const
{
    std::vector<Arity> indexes=m_crtActions->at(actionKey);
    if(indexes.size()==0)
    {
        return { (*(this->m_son))[0]->applyFirstPriv(*(m_actionToIdentifications->at(actionKey))), isSymetric()};
    }
    else
    {
        return { std::dynamic_pointer_cast<const ASubTheorem>(applyAnnexe(actionKey,theorem, indexes)), isSymetric()};
    }
}

template<SubRuleProperty SubPropertyType>
ptr<IISubTheoremFormula> Rule<SubPropertyType>::applyAnnexe(const size_t &actionKey, const ptr<IISubTheoremFormula> &theorem,
                                                    std::vector<Arity> &indexes) const
{
    auto theoremCast=std::dynamic_pointer_cast<const ASubTheorem>(theorem); //?? cast not only to ASubTheorem
    if(indexes.size()==0)
    {
        return (*(this->m_son))[0]->applyPriv(*(m_actionToIdentifications->at(actionKey)));
    }
    else
    {
        return theoremCast->ruleApply(*this,indexes,actionKey);
    }
}

template<SubRuleProperty SubPropertyType>
void Rule<SubPropertyType>::unapply() const
{
    clearIdentifications();
}

template<SubRuleProperty SubPropertyType>
inline bool Rule<SubPropertyType>::isSymetric() const
{
    return false;
}

template<SubRuleProperty SubPropertyType>
inline ptr<Rule<SubPropertyType>> Rule<SubPropertyType>::getReciprocal() const
{
    return std::make_shared<const Rule<SubPropertyType>>(name()+"_Recip",(*this)[1],(*this)[0]);
}

template<SubRuleProperty SubPropertyType>
inline ptr<Rule<SubPropertyType>> Rule<SubPropertyType>::getTrueEquivalent() const
{
    return std::make_shared<const Rule<SubPropertyType>>(name() + "_True",
        std::make_shared<const SubRule<ConstBoolean>>(true),
        std::make_shared<const SubRule<SubPropertyType>>((*this)[0],(*this)[1]) 
        );
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
                propToIdentify=((*std::dynamic_pointer_cast<const ASubPureTheorem>(propToIdentify))[index]);
                break;
            }
            case IISubTheoremFormula::SUBIMPURETH:
            {
                propToIdentify=((*std::dynamic_pointer_cast<const ASubImpureTheorem>(propToIdentify))[index]);
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
    if(impl->identifyPriv(std::dynamic_pointer_cast<const ASubTheorem>(propToIdentify),dbVarProp))
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
    m_crtActions->clear();
    m_actionToIdentifications->clear();
}

}

#endif // RULE_H
