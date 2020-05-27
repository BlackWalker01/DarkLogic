/*===--- subrule.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines SubRule class in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "subrule.h"
#include "theorem.h"
#include "Formula/Arithmetic/allarithformula.h"
#include "allproposition.h"
#include "logic.h"

using namespace N_DarkLogic;

ptr<ValueTypeObject> N_DarkLogic::createSubRule(const std::string &name, std::vector<OperatorOrdering> &opeList,
                                            std::vector<OperatorOrdering> &orderedOpeList,
                                                std::vector<std::shared_ptr<VariableContainer>> &varList)
{
    if(orderedOpeList.size())
    {        
        auto ope=orderedOpeList[0].ope;
        if(ope->name()==HYP)
        {
            orderedOpeList.erase(orderedOpeList.begin());
            opeList.erase(opeList.begin());
            std::vector<OperatorOrdering> topOpeList, topOrderedOpeList, queueOpeList, queueOrderedOpeList;
            std::vector<ptr<IISubRuleFormula>> subProps;
            for(size_t k=0;k<ope->arity();k++)
            {
                if (orderedOpeList.size())
                {
                    //split between topOrderedOpeList and queueOrderedOpeList
                    std::unordered_map<ptr<IOperator>, OperatorOrdering> hashLeftOpe;
                    std::unordered_map<ptr<IOperator>, OperatorOrdering> hashRightOpe;
                    for (size_t i = 0; i < orderedOpeList.size(); i++)
                    {
                        if (orderedOpeList[i].argIndex == k)
                        {
                            topOrderedOpeList.push_back(orderedOpeList[i]);
                            auto ope = &(orderedOpeList[i]);
                            hashLeftOpe[ope->ope] = *ope;
                        }
                        else
                        {
                            queueOrderedOpeList.push_back(orderedOpeList[i]);
                            auto ope = &(orderedOpeList[i]);
                            hashRightOpe[ope->ope] = *ope;
                        }
                    }

                    //split between topOpeList and queueOpeList
                    for (size_t i = 0; i < opeList.size(); i++)
                    {
                        auto it = hashLeftOpe.find(opeList[i].ope);
                        if (it != hashLeftOpe.end())
                        {
                            topOpeList.push_back(opeList[i]);
                        }
                        else
                        {
                            queueOpeList.push_back(opeList[i]);
                        }
                    }

                    orderedOpeList = queueOrderedOpeList;
                    opeList = queueOpeList;
                }

                //top sub-SubTheorem
                subProps.push_back(std::dynamic_pointer_cast<const IISubRuleFormula>
                                   (createSubRule(name+ope->symbol()+sizeToString(k),topOpeList,topOrderedOpeList,varList)));
                topOrderedOpeList.clear();
                queueOrderedOpeList.clear();
            }

            switch (ope->name())
            {
                case HYP:
                {
                    ptr<ASubRule> subPropCast=nullptr;
                    std::vector<ptr<ASubRule>> subPropCasts;
                    for(auto subProp: subProps)
                    {
                        if((subPropCast=std::static_pointer_cast<const ASubRule>(subProp)))
                        {
                            subPropCasts.push_back(subPropCast);
                        }
                        else
                        {
                            throw std::runtime_error("Expected SubRule in hypothesis operator");
                        }
                    }
                    return std::make_shared<const SubRule<Hyp<ASubRule>>>(subPropCasts);
                }
                default:
                {
                    throw std::runtime_error("Empty SubRule");
                }
            }
        }
        else
        {
            switch(ope->arity())
            {
                case 1:
                {
                    std::vector<OperatorOrdering> queueOpeList(opeList.begin()+1,opeList.end());
                    std::vector<OperatorOrdering> queueOrderedOpeList(orderedOpeList.begin()+1,orderedOpeList.end());
                    switch(ope->name())
                    {
                        case NOT:
                        {
                            auto subOpe=createSubRule(name+"!",queueOpeList,queueOrderedOpeList,varList);
                            switch (subOpe->valueType())
                            {
                                case BOOL_TYPE:
                                {
                                    auto subOpeCast=std::static_pointer_cast<const ASubRule>(subOpe);
                                    return std::make_shared<const SubRule<Not<ASubRule>>>(subOpeCast);
                                }
                                default:
                                {
                                    throw std::runtime_error("Expected boolean variable just after not operator");
                                }
                            }
                        }
                        default:
                        {
                            throw std::runtime_error("Only operators of arity one in this part of formula");
                        }
                    }
                    break;
                }
                case 2:
                {
                    std::vector<OperatorOrdering> queueOrderedOpeList(orderedOpeList.begin()+1,orderedOpeList.end());
                    std::vector<OperatorOrdering> leftOpeList;
                    std::vector<OperatorOrdering> rightOpeList;
                    std::vector<OperatorOrdering> leftOrderedOpeList;
                    std::vector<OperatorOrdering> rightOrderedOpeList;
                    if(ope->arity()==2)
                    {
                        for(unsigned int k=0; k<opeList.size();k++)
                        {
                            if(ope==(opeList[k].ope))
                            {
                                leftOpeList.insert(leftOpeList.begin(),opeList.begin(),opeList.begin()+k);
                                rightOpeList.insert(rightOpeList.begin(),opeList.begin()+k+1,opeList.end());
                                break;
                            }
                        }

                        //store remaining operators to the left of current operator
                        std::unordered_map<ptr<IOperator>,OperatorOrdering> hashLeftOpe;
                        for(unsigned int i=0;i<leftOpeList.size();i++)
                        {
                            auto crtOpe=&(leftOpeList[i]);
                            hashLeftOpe[crtOpe->ope]=*crtOpe;
                        }

                        //store remaining operators to the right of current operator
                        std::unordered_map<ptr<IOperator>,OperatorOrdering> hashRightOpe;
                        for(unsigned int i=0;i<rightOpeList.size();i++)
                        {
                            auto crtOpe=&(rightOpeList[i]);
                            hashRightOpe[crtOpe->ope]=*crtOpe;
                        }

                        //separate operators in queueOrderedOpeList to the left and to the right and order them in each list
                        for(unsigned int i=0;i<queueOrderedOpeList.size();i++)
                        {
                            auto crtOpe=&(queueOrderedOpeList[i]);
                            auto itLeft=hashLeftOpe.find(crtOpe->ope);
                            if(itLeft!=hashLeftOpe.end())
                            {
                                leftOrderedOpeList.push_back(*crtOpe);
                                continue;
                            }
                            auto itRight=hashRightOpe.find(crtOpe->ope);
                            if(itRight!=hashRightOpe.end())
                            {
                                rightOrderedOpeList.push_back(*crtOpe);
                                continue;
                            }
                        }
                    }

                    switch(ope->name())
                    {
                        case AND:
                        {
                            auto leftOpe=createSubRule(name+"&&L",leftOpeList,leftOrderedOpeList,varList);
                            auto rightOpe=createSubRule(name+"&&R",rightOpeList,rightOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case BOOL_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case BOOL_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubRule>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubRule>(rightOpe);
                                            return std::make_shared<const SubRule<And<ASubRule>>>
                                            (leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("And operator expects boolean variable");
                                        }
                                    }
                                }
                                default:
                                {
                                    throw std::runtime_error("And operator expects boolean variable");
                                }
                            }
                        }
                        case EQUIVALENT:
                        {
                            auto leftOpe=createSubRule(name+"<=>L",leftOpeList,leftOrderedOpeList,varList);
                            auto rightOpe=createSubRule(name+"<=>R",rightOpeList,rightOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case BOOL_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case BOOL_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubRule>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubRule>(rightOpe);
                                            return std::make_shared<const SubRule<Equivalent<ASubRule>>>
                                            (leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("Equivalent operator does not handle bool variable");
                                        }
                                    }
                                }
                                default:
                                {
                                    throw std::runtime_error("Equivalent operator does not handle bool variable");
                                }
                            }
                        }
                        case IMPLICATION:
                        {
                            auto leftOpe=createSubRule(name+"=>L",leftOpeList,leftOrderedOpeList,varList);
                            auto rightOpe=createSubRule(name+"=>R",rightOpeList,rightOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case BOOL_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case BOOL_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubRule>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubRule>(rightOpe);
                                            return std::make_shared<const SubRule<Implication<ASubRule >>>
                                            (leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("Implication operator expects boolean variable");
                                        }
                                    }
                                }
                                default:
                                {
                                    throw std::runtime_error("Implication operator expects boolean variable");
                                }
                            }
                        }
                        case OR:
                        {
                            auto leftOpe=createSubRule(name+"||L",leftOpeList,leftOrderedOpeList,varList);
                            auto rightOpe=createSubRule(name+"||R",rightOpeList,rightOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case BOOL_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case BOOL_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubRule>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubRule>(rightOpe);
                                            return std::make_shared<const SubRule<Or<ASubRule >>>
                                            (leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("Or operator expects boolean variable");
                                        }
                                    }
                                }
                                default:
                                {
                                    throw std::runtime_error("Or operator expects boolean variable");
                                }
                            }
                        }
                        case SETEQUAL:
                        {
                            auto firstVar=varList[0];
                            varList.erase(varList.begin());
                            auto rightOpe=createSubRule(name+"=R",rightOpeList,rightOrderedOpeList,varList);
                            varList.insert(varList.begin(),firstVar);
                            if(firstVar->type==VALUE_TYPE::VOID_TYPE)
                            {
                                firstVar->type=rightOpe->valueType();
                            }
                            else if(firstVar->type!=rightOpe->valueType())
                            {
                                throw std::runtime_error(firstVar->var->name()+" is not of the same type of right operand");
                            }
                            auto leftOpe=createSubRule(name+"=L",leftOpeList,leftOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case NATURAL_INT_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case NATURAL_INT_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const SubArithmeticRule<NaturalInteger>>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubArithmeticRule<size_t>>(rightOpe);
                                            return std::make_shared<const SubArithmeticRule<SetEqual<SubArithmeticRule<NaturalInteger> >>>(
                                                                                                                leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("SetEqual operator does not handle bool variable");
                                        }
                                    }
                                }
                                default:
                                {
                                    throw std::runtime_error("SetEqual operator does not handle bool variable");
                                }
                            }
                        }
                        case EQUAL:
                        {
                            auto leftOpe=createSubRule(name+"==L",leftOpeList,leftOrderedOpeList,varList);
                            auto rightOpe=createSubRule(name+"==R",rightOpeList,rightOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case NATURAL_INT_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case NATURAL_INT_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubArithmeticRule<size_t>>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubArithmeticRule<size_t>>(rightOpe);
                                            return std::make_shared<const SubRule<Equal<ASubArithmeticRule<size_t>, ASubArithmeticRule<size_t> >>>
                                                                    (leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("Equal operator does not handle bool variable");
                                        }
                                    }
                                }
                                default:
                                {
                                    throw std::runtime_error("Equal operator does not handle bool variable");
                                }
                            }
                        }
                        case BELONGSTO:
                        {
                            if(varList.size()<2)
                            {
                                throw std::runtime_error("€ operator expects 2 arguments but got only "+sizeToString(varList.size()));
                            }
                            std::shared_ptr<VariableContainer> firstVar=nullptr;
                            ptr<ValueTypeObject> leftOpe=nullptr;
                            ptr<ValueTypeObject> rightOpe=nullptr;
                            if(leftOpeList.size()==0)
                            {
                                firstVar=varList[0];
                                varList.erase(varList.begin());
                                rightOpe=createSubRule(name+"€R",rightOpeList,rightOrderedOpeList,varList);
                                varList.insert(varList.begin(),firstVar);
                                if(firstVar->type==VALUE_TYPE::VOID_TYPE)
                                {
                                    switch (rightOpe->valueType())
                                    {
                                        case NATURAL_INTEGERSET_TYPE:
                                        {
                                            firstVar->type=NATURAL_INT_TYPE;
                                            break;
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("BelongsTo operator expects set as right operand");
                                        }
                                    }
                                    firstVar->init();
                                }
                                else if(firstVar->type!=rightOpe->valueType())
                                {
                                    throw std::runtime_error(firstVar->var->name()+" is not of the same type of right operand in € operator");
                                }
                                leftOpe=createSubRule(name+"€L",leftOpeList,leftOrderedOpeList,varList);
                            }
                            else
                            {
                                leftOpe=createSubRule(name+"€L",leftOpeList,leftOrderedOpeList,varList);
                                rightOpe=createSubRule(name+"€R",rightOpeList,rightOrderedOpeList,varList);
                            }
                            switch(leftOpe->valueType())
                            {
                                case NATURAL_INT_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case NATURAL_INTEGERSET_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubArithmeticRule<size_t>>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubArithmeticRule<NaturalIntegerSet>>(rightOpe);
                                            return std::make_shared<const SubRule<BelongsTo<ASubArithmeticRule<size_t>, ASubArithmeticRule<NaturalIntegerSet> >>>
                                                    (leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("BelongsTo operator does not handle bool variable");
                                        }
                                    }
                                }
                                default:
                                {
                                    throw std::runtime_error("BelongsTo operator does not handle bool variable");
                                }
                            }
                        }
                        case LET:
                        {
                            auto leftOpe=createSubRule(name+"letL",leftOpeList,leftOrderedOpeList,varList);
                            auto rightOpe=createSubRule(name+"letR",rightOpeList,rightOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case VOID_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case BOOL_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubArithmeticRule<void>>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubRule>(rightOpe);
                                            return std::make_shared<const SubRule<Let<ASubArithmeticRule<void>,ASubRule>>>
                                                                                            (leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("Let operator handles only boolean as right operand");
                                        }
                                    }
                                }
                                case BOOL_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case BOOL_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubRule>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubRule>(rightOpe);
                                            return std::make_shared<const SubRule<Let<ASubRule,ASubRule>>>
                                                                                (leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("Let operator handles only boolean as right operand");
                                        }
                                    }
                                }
                                default:
                                {
                                    throw std::runtime_error("Let operator does not handle numbers as left operand");
                                }
                            }
                        }
                        case PLUS:
                        {
                            auto leftOpe=createSubRule(name+"+L",leftOpeList,leftOrderedOpeList,varList);
                            auto rightOpe=createSubRule(name+"+R",rightOpeList,rightOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case NATURAL_INT_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case NATURAL_INT_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubArithmeticRule<size_t>>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubArithmeticRule<size_t>>(rightOpe);
                                            return std::make_shared<const SubArithmeticRule<Plus<ASubArithmeticRule<size_t>,
                                                    ASubArithmeticRule<size_t> >>>(leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("Plus operator does not handle bool variable");
                                        }
                                    }
                                }
                                default:
                                {
                                    throw std::runtime_error("Plus operator does not handle bool variable");
                                }
                            }
                        }
                        default:
                        {
                            throw std::runtime_error("Do not handle boolean operators in arithmetic formula");
                        }
                    }
                }

                default:
                {
                    throw std::runtime_error("Empty SubRule");
                }
            }

        }
    }
    else if(varList.size())
    {
        std::shared_ptr<AbstractTerm> var=varList[0]->var;
        varList.erase(varList.begin());
        if(var->isConstant())
        {
            switch(var->valueType())
            {
                case VALUE_TYPE::BOOL_TYPE:
                {
                    return std::make_shared<const SubRule<ConstBoolean>>(
                                            std::static_pointer_cast<ConstBoolean>(var)->evaluate());
                }
                case VALUE_TYPE::NATURAL_INT_TYPE:
                {
                    return std::make_shared<const ConstSubArithmeticRule<ConstNaturalInteger>>(
                        std::static_pointer_cast<ConstNaturalInteger>(var)->evaluate());
                }
                case VALUE_TYPE::NATURAL_INTEGERSET_TYPE:
                {
                    return std::make_shared<const ConstSubArithmeticTheorem<ConstSet<NaturalIntegerSet>>>(
                                                std::static_pointer_cast<ConstSet<NaturalIntegerSet>>(var)->evaluate());
                }
                case VALUE_TYPE::BOOLEANSET_TYPE:
                {
                    return std::make_shared<const ConstSubArithmeticTheorem<ConstSet<BooleanSet>>>(
                                                std::static_pointer_cast<ConstSet<BooleanSet>>(var)->evaluate());
                }
                default:
                {
                    throw std::runtime_error("No bool variable in arithmetic formula");
                }
            }
        }
        else
        {
            switch(var->valueType())
            {
                case VALUE_TYPE::BOOL_TYPE:
                {
                    return std::make_shared<const SubRule<Boolean>>(
                                            std::static_pointer_cast<Boolean>(var));
                }
                case VALUE_TYPE::NATURAL_INT_TYPE:
                {
                    return std::make_shared<const SubArithmeticRule<NaturalInteger>>(
                                        std::static_pointer_cast<NaturalInteger>(var));
                }

                case VALUE_TYPE::NATURAL_INTEGERSET_TYPE:
                {
                    return std::make_shared<const SubArithmeticRule<VarSet<NaturalIntegerSet>>>(
                                                std::static_pointer_cast<VarSet<NaturalIntegerSet>>(var));
                }
                case VALUE_TYPE::BOOLEANSET_TYPE:
                {
                    return std::make_shared<const SubArithmeticRule<VarSet<BooleanSet>>>(
                                                std::static_pointer_cast<VarSet<BooleanSet>>(var));
                }
                default:
                {
                    throw std::runtime_error("No bool variable in arithmetic formula");
                }
            }
        }
    }
    else
    {
        throw std::runtime_error("Empty SubRule");
    }
    return nullptr;
}


std::vector<State> getStates(const std::vector < ptr<ASubRule>>& subProps)
{
    std::vector<State> ret;
    for (const auto& subProp : subProps)
    {
        ret.push_back(subProp->getState());
    }
    return ret;
}

/**---------------------------------------------------------------
 * Constructor methods from subProperties
 * ---------------------------------------------------------------
 */

template<>
SubRule<And<ASubRule>>::SubRule(const ptr<ASubRule>& leftSubProp,
                                         const ptr<ASubRule>& rightSubProp):
    m_son(std::make_unique<And<ASubRule>>(leftSubProp,rightSubProp)),
    m_extVars(leftSubProp->getExtVars(), rightSubProp->getExtVars()),
    m_state(AND, leftSubProp->getState(), rightSubProp->getState())
{

}

template<>
SubRule<Equivalent<ASubRule>>::SubRule(const ptr<ASubRule>& leftSubProp,
                                                const ptr<ASubRule>& rightSubProp):
    m_son(std::make_unique<Equivalent<ASubRule>>(leftSubProp,rightSubProp)),
    m_extVars(leftSubProp->getExtVars(), rightSubProp->getExtVars()),
    m_state(EQUIVALENT, leftSubProp->getState(), rightSubProp->getState())
{

}

SubRule<Hyp<ASubRule>>::SubRule(const std::vector<ptr<ASubRule>>& subProps):
    m_son(std::make_unique<Hyp<ASubRule>>(subProps)), m_extVars(getDbVarFromRules(subProps)),
    m_state(HYP, getStates(subProps))
{

}

template<>
SubRule<Implication<ASubRule>>::SubRule(const ptr<ASubRule>& leftSubProp,
                                                 const ptr<ASubRule>& rightSubProp):
    m_son(std::make_unique<Implication<ASubRule>>(leftSubProp,rightSubProp)),
    m_extVars(leftSubProp->getExtVars(), rightSubProp->getExtVars()),
    m_state(IMPLICATION, leftSubProp->getState(), rightSubProp->getState())
{

}

SubRule<Not<ASubRule>>::SubRule(const ptr<ASubRule>& subProp):
    m_son(std::make_unique<Not<ASubRule>>(subProp)),
    m_extVars(subProp->getExtVars()),
    m_state(NOT, subProp->getState())
{

}

template<>
SubRule<Or<ASubRule>>::SubRule(const ptr<ASubRule> &leftSubProp, const ptr<ASubRule> &rightSubProp):
    m_son(std::make_unique<Or<ASubRule>>(leftSubProp,rightSubProp)),
    m_extVars(leftSubProp->getExtVars(), rightSubProp->getExtVars()),
    m_state(OR, leftSubProp->getState(), rightSubProp->getState())
{

}

SubRule<Boolean>::SubRule(const std::shared_ptr<Boolean>& son):
    m_son(son), m_extVars(son),
    m_state(son->id(), BOOL_TYPE)
{

}

SubRule<ConstBoolean>::SubRule(const bool& val):
    m_son(std::make_unique<ConstBoolean>(val)),
    m_state(BOOL_TYPE, val)
{

}


/**---------------------------------------------------------------
 * Evaluate methods
 * ---------------------------------------------------------------
 */

template<SubRuleProperty SubPropertyType>
bool SubRule<SubPropertyType>::evaluate() const
{
    return m_son->evaluate();
}

bool SubRule<Hyp<ASubRule>>::evaluate() const
{
    return m_son->evaluate();
}

bool SubRule<Not<ASubRule>>::evaluate() const
{
    return m_son->evaluate();
}

bool SubRule<Boolean>::evaluate() const
{
    return m_son->evaluate();
}

bool SubRule<ConstBoolean>::evaluate() const
{
    return m_son->evaluate();
}

/**---------------------------------------------------------------
 * getState methods
 * ---------------------------------------------------------------
 */

template<SubRuleProperty SubPropertyType>
const State& SubRule<SubPropertyType>::getState() const
{
    return m_state;
}

const State& SubRule<Hyp<ASubRule>>::getState() const
{
    return m_state;
}

const State& SubRule<Not<ASubRule>>::getState() const
{
    return m_state;
}

const State& SubRule<Boolean>::getState() const
{
    return m_state;
}

const State& SubRule<ConstBoolean>::getState() const
{
    return m_state;
}


/**---------------------------------------------------------------
 * toString methods
 * ---------------------------------------------------------------
 */
template<SubRuleProperty SubPropertyType>
std::string SubRule<SubPropertyType>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

std::string SubRule<Hyp<ASubRule>>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

std::string SubRule<Not<ASubRule>>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

std::string SubRule<Boolean>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

std::string SubRule<ConstBoolean>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

/**---------------------------------------------------------------
 * getExtVars methods
 * ---------------------------------------------------------------
 */

const DbVar* SubRule<Hyp<ASubRule>>::getExtVars() const
{
    return &m_extVars;
}

const DbVar* SubRule<Not<ASubRule>>::getExtVars() const
{
    return &m_extVars;
}

const DbVar* SubRule<Boolean>::getExtVars() const
{
    return &m_extVars;
}

const DbVar* SubRule<ConstBoolean>::getExtVars() const
{
    return nullptr;
}


/**---------------------------------------------------------------
 * getSon methods
 * ---------------------------------------------------------------
 */
template<SubRuleProperty SubPropertyType>
const SubPropertyType &SubRule<SubPropertyType>::getSon() const
{
    return *(m_son.get());
}

const Hyp<ASubRule>& SubRule<Hyp<ASubRule>>::getSon() const
{
    return *(m_son.get());
}

const Not<ASubRule>& SubRule<Not<ASubRule>>::getSon() const
{
    return *(m_son.get());
}

const Boolean& SubRule<Boolean>::getSon() const
{
    return *(m_son.get());
}

const ConstBoolean& SubRule<ConstBoolean>::getSon() const
{
    return *(m_son.get());
}

/**---------------------------------------------------------------
 * operator[] methods
 * ---------------------------------------------------------------
 */
template<SubRuleProperty SubPropertyType>
const ptr<ASubRule>& SubRule<SubPropertyType>::operator[](const size_t& index) const
{
    return (*m_son)[index];
}

const ptr<ASubRule>& SubRule<Hyp<ASubRule>>::operator[](const size_t& index) const
{
    return (*m_son)[index];
}

const ptr<ASubRule>& SubRule<Not<ASubRule>>::operator[](const size_t&) const
{
    return (*m_son)[0];
}

/**---------------------------------------------------------------
 * variableId methods
 * ---------------------------------------------------------------
 */
IDVar SubRule<Boolean>::variableId() const
{
    return m_son->id();
}

/**---------------------------------------------------------------
 * arity methods
 * ---------------------------------------------------------------
 */
template<SubRuleProperty SubPropertyType>
size_t SubRule<SubPropertyType>::arity() const
{
    return m_son->arity();
}

size_t SubRule<Not<ASubRule>>::arity() const
{
    return m_son->arity();
}

size_t SubRule<Hyp<ASubRule>>::arity() const
{
    return m_son->arity();
}

size_t SubRule<Boolean>::arity() const
{
    return 0;
}

size_t SubRule<ConstBoolean>::arity() const
{
    return 0;
}


/**---------------------------------------------------------------
 * identifyPriv methods
 * ---------------------------------------------------------------
 */
template<>
bool SubRule<And<ASubRule>>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const
{
    if(prop->type()==PropType::AND_PROP)
    {
        auto propCast=std::static_pointer_cast<const SubTheorem<And<ASubTheorem>>>(prop);
        return (*m_son)[0]->identifyPriv(propCast->getSon()[0],dbVarProp) && (*m_son)[1]->identifyPriv(propCast->getSon()[1],dbVarProp);
    }
    return false;
}

template<>
bool SubRule<Equivalent<ASubRule>>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const
{
    if(prop->type()==PropType::EQUIV_PROP)
    {
        auto propCast=std::static_pointer_cast<const SubTheorem<Equivalent<ASubTheorem>>>(prop);
        return (*m_son)[0]->identifyPriv(propCast->getSon()[0],dbVarProp) && (*m_son)[1]->identifyPriv(propCast->getSon()[1],dbVarProp);
    }
    return false;
}

template<>
bool SubRule<Implication<ASubRule>>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const
{
    if(prop->type()==PropType::IMPL_PROP)
    {
        auto propCast=std::static_pointer_cast<const SubTheorem<Implication<ASubTheorem>>>(prop);
        return (*m_son)[0]->identifyPriv(propCast->getSon()[0],dbVarProp) && (*m_son)[1]->identifyPriv(propCast->getSon()[1],dbVarProp);
    }
    return false;
}

bool SubRule<Hyp<ASubRule>>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const
{
    if(prop->type()==PropType::HYP_PROP)
    {
        auto propCast=std::static_pointer_cast<const SubTheorem<Hyp<ASubTheorem>>>(prop);
        if(propCast->arity()>=m_son->arity()-1)
        {
            bool ret=false;
            bool hasAlreadyHyp = false;
            size_t subThIdx = 0;
            //Identify all properties before conclusion
            for(size_t k=0;k<m_son->arity()-1;k++)
            {
                //if is HYP variable
                if ((*m_son)[k]->isHypProp())
                {
                    //only one HYP variable can be automatically identified
                    if (hasAlreadyHyp)
                    {
                        return false;
                    }
                    hasAlreadyHyp = true;
                    size_t nbToIdentify = propCast->arity() - m_son->arity() + 1;
                    IDVar idHypVar = (*m_son)[k]->variableId();
                    if (dbVarProp.containsHyp(idHypVar))
                    {
                        //check if old mapping is the same as the current one
                        auto props = dbVarProp.getHypAssoc(idHypVar);
                        if (nbToIdentify != props.size())
                        {
                            return false;
                        }
                        for (size_t i = 0; i < nbToIdentify; i++)
                        {
                            ret = *(props[i]) == *(propCast->getSon()[subThIdx]);
                            if (!ret)
                            {
                                return false;
                            }
                            subThIdx++;
                        }
                    }
                    else
                    {
                        dbVarProp.insertHypEmpty(idHypVar);
                        //map HYP variable with prop subProperties
                        for (size_t i = 0; i < nbToIdentify; i++)
                        {
                            dbVarProp.insertHypAssoc(idHypVar, propCast->getSon()[subThIdx]);
                            subThIdx++;
                        }
                    }
                }
                //general case
                else
                {
                    ret = (*m_son)[k]->identifyPriv(propCast->getSon()[subThIdx], dbVarProp);
                    if (!ret)
                    {
                        return false;
                    }
                    subThIdx++;
                }                
            }            

            //conclusion case
            ret=(*m_son)[m_son->arity()-1]->identifyPriv(propCast->getSon()[propCast->arity()-1],dbVarProp);
            return ret;
        }
    }
    else if(m_son->arity()==2)
    {
        IDVar idHypVar=(*m_son)[0]->variableId();
        if (dbVarProp.containsHyp(idHypVar))
        {
            return false;
        }
        else
        {
            dbVarProp.insertHypEmpty(idHypVar);
            return (*m_son)[1]->identifyPriv(prop, dbVarProp);
        }        
    }
    return false;
}

bool SubRule<Not<ASubRule>>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const
{
    if(prop->type()==PropType::NOT_PROP)
    {
        auto propCast=std::static_pointer_cast<const SubTheorem<Not<ASubTheorem>>>(prop);
        return (*m_son)[0]->identifyPriv(propCast->getSon()[0],dbVarProp);
    }
    return false;
}

template<>
bool SubRule<Or<ASubRule>>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const
{
    if(prop->type()==PropType::OR_PROP)
    {
        auto propCast=std::static_pointer_cast<const SubTheorem<Or<ASubTheorem>>>(prop);
        return (*m_son)[0]->identifyPriv(propCast->getSon()[0],dbVarProp) && (*m_son)[1]->identifyPriv(propCast->getSon()[1],dbVarProp);
    }
    return false;
}

bool SubRule<Boolean>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const
{
    if(!dbVarProp.contains(m_son->id()))
    {
        dbVarProp[m_son->id()]=prop;
        return true;
    }
    else
    {
        auto exPropCast=std::static_pointer_cast<const ASubTheorem>(dbVarProp[m_son->id()]);
        if(exPropCast)
        {
            return *prop==*exPropCast;
        }
        else
        {
            return false;
        }

    }
}

bool SubRule<ConstBoolean>::identifyPriv(const ptr<ASubTheorem> &prop, DbVarProp&) const
{
    return (*this)==*prop;
}


/**---------------------------------------------------------------
 * applyPriv methods
 * ---------------------------------------------------------------
 */

ptr<ASubTheorem> SubRule<Hyp<ASubRule>>::applyPriv(DbVarProp& dbVarProp) const
{
    std::vector<ptr<ASubTheorem>> ret;
    //Handle all variable in Hyp operator before implication
    for(size_t k=0;k<m_son->arity()-1;k++)
    {
        if((*m_son)[k]->isHypProp())
        {
            IDVar idHypVar=(*m_son)[k]->variableId();
            std::vector<ptr<ASubTheorem>> hypAssoc=dbVarProp.getHypAssoc(idHypVar);
            for(size_t k=0;k<hypAssoc.size();k++)
            {
                ret.push_back(hypAssoc[k]);
            }
        }
        else
        {
            ret.push_back((*m_son)[k]->applyPriv(dbVarProp));
        }
    }

    if(ret.size()>0)
    {
        ret.push_back((*m_son)[m_son->arity()-1]->applyPriv(dbVarProp));
        return Logic::make_theorem_formula<SubTheorem<Hyp<ASubTheorem>>>(ret);
    }
    else
    {
        return (*m_son)[m_son->arity()-1]->applyPriv(dbVarProp);
    }
}

ptr<ASubTheorem> SubRule<Hyp<ASubRule>>::applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    std::vector<ptr<ASubTheorem>> ret;
    //Handle all variable in Hyp operator before implication
    for (size_t k = 0; k < m_son->arity() - 1; k++)
    {
        if ((*m_son)[k]->isHypProp())
        {
            IDVar idHypVar = (*m_son)[k]->variableId();
            std::vector<ptr<ASubTheorem>> hypAssoc = dbVarProp.getHypAssoc(idHypVar);
            for (size_t k = 0; k < hypAssoc.size(); k++)
            {
                ret.push_back(hypAssoc[k]);
            }
        }
        else
        {
            ret.push_back((*m_son)[k]->applyPriv(dbVarProp, logicIdx));
        }
    }

    if (ret.size() > 0)
    {
        ret.push_back((*m_son)[m_son->arity() - 1]->applyPriv(dbVarProp, logicIdx));
        return Logic::make_theorem_formula<SubTheorem<Hyp<ASubTheorem>>>(logicIdx, ret);
    }
    else
    {
        return (*m_son)[m_son->arity() - 1]->applyPriv(dbVarProp, logicIdx);
    }
}

ptr<ASubTheorem> SubRule<Not<ASubRule>>::applyPriv(DbVarProp& dbVarProp) const
{
    return Logic::make_theorem_formula<SubTheorem<Not<ASubTheorem>>>((*m_son)[0]->applyPriv(dbVarProp));
}

ptr<ASubTheorem> SubRule<Not<ASubRule>>::applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<SubTheorem<Not<ASubTheorem>>>(logicIdx, (*m_son)[0]->applyPriv(dbVarProp, logicIdx));
}

ptr<ASubTheorem> SubRule<Boolean>::applyPriv(DbVarProp& dbVarProp) const
{
    return std::static_pointer_cast<const ASubTheorem>(dbVarProp[m_son->id()]);
}

ptr<ASubTheorem> SubRule<Boolean>::applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return std::static_pointer_cast<const ASubTheorem>(dbVarProp[m_son->id()]);
}

ptr<ASubTheorem> SubRule<ConstBoolean>::applyPriv(DbVarProp&) const
{
    return Logic::make_theorem_formula<SubTheorem<ConstBoolean>>(this->evaluate());
}

ptr<ASubTheorem> SubRule<ConstBoolean>::applyPriv(DbVarProp&, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<SubTheorem<ConstBoolean>>(logicIdx, this->evaluate());
}


/**---------------------------------------------------------------
 * applyFirstPriv methods
 * ---------------------------------------------------------------
 */


ptr<ASubTheorem> SubRule<Hyp<ASubRule>>::applyFirstPriv(DbVarProp& dbVarProp) const
{
    std::vector<ptr<ASubTheorem>> ret;
    //Handle all variable in Hyp operator before HYP variable
    for(size_t k=0;k<m_son->arity()-1;k++)
    {
        if((*m_son)[k]->isHypProp())
        {
            IDVar idHypVar=(*m_son)[k]->variableId();
            std::vector<ptr<ASubTheorem>> hypAssoc=dbVarProp.getHypAssoc(idHypVar);
            for(size_t k=0;k<hypAssoc.size();k++)
            {
                ret.push_back(hypAssoc[k]);
            }
        }
        else
        {
            ret.push_back((*m_son)[k]->applyPriv(dbVarProp));
        }
    }

    if(ret.size()>0)
    {
        ret.push_back((*m_son)[m_son->arity()-1]->applyPriv(dbVarProp));
        return Logic::make_theorem_formula<SubTheorem<Hyp<ASubTheorem>>>(ret);
    }
    else
    {
        return (*m_son)[m_son->arity()-1]->applyFirstPriv(dbVarProp);
    }
}

ptr<ASubTheorem> SubRule<Hyp<ASubRule>>::applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    std::vector<ptr<ASubTheorem>> ret;
    //Handle all variable in Hyp operator before HYP variable
    for (size_t k = 0; k < m_son->arity() - 1; k++)
    {
        if ((*m_son)[k]->isHypProp())
        {
            IDVar idHypVar = (*m_son)[k]->variableId();
            std::vector<ptr<ASubTheorem>> hypAssoc = dbVarProp.getHypAssoc(idHypVar);
            for (size_t k = 0; k < hypAssoc.size(); k++)
            {
                ret.push_back(hypAssoc[k]);
            }
        }
        else
        {
            ret.push_back((*m_son)[k]->applyPriv(dbVarProp, logicIdx));
        }
    }

    if (ret.size() > 0)
    {
        ret.push_back((*m_son)[m_son->arity() - 1]->applyPriv(dbVarProp, logicIdx));
        return Logic::make_theorem_formula<Theorem<Hyp<ASubTheorem>> > (logicIdx, ret);
    }
    else
    {
        return (*m_son)[m_son->arity() - 1]->applyFirstPriv(dbVarProp, logicIdx);
    }
}



ptr<ASubTheorem> SubRule<Not<ASubRule>>::applyFirstPriv(DbVarProp& dbVarProp) const
{
    return Logic::make_theorem_formula<Theorem<Not<ASubTheorem>>>((*m_son)[0]->applyPriv(dbVarProp));
}

ptr<ASubTheorem> SubRule<Not<ASubRule>>::applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<Theorem<Not<ASubTheorem>>>(logicIdx, (*m_son)[0]->applyPriv(dbVarProp, logicIdx));
}

ptr<ASubTheorem> SubRule<Boolean>::applyFirstPriv(DbVarProp& dbVarProp) const
{
    return std::static_pointer_cast<const ASubTheorem>(dbVarProp[m_son->id()])->copyTheorem();
}

ptr<ASubTheorem> SubRule<Boolean>::applyFirstPriv(DbVarProp& dbVarProp, const size_t&) const
{
    return std::static_pointer_cast<const ASubTheorem>(dbVarProp[m_son->id()])->copyTheorem();
}

ptr<ASubTheorem> SubRule<ConstBoolean>::applyFirstPriv(DbVarProp&) const
{
    return Logic::make_theorem_formula<Theorem<ConstBoolean>>(this->evaluate());
}

ptr<ASubTheorem> SubRule<ConstBoolean>::applyFirstPriv(DbVarProp&, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<Theorem<ConstBoolean>>(logicIdx, this->evaluate());
}

/**---------------------------------------------------------------
 * SubRule::getEquivalentRules() methods
 * ---------------------------------------------------------------
 */
template<SubRuleProperty SubPropertyType>
std::vector<ptr<ASubRule>> SubRule<SubPropertyType>::getEquivalentRules() const
{
    std::vector<ptr<ASubRule>> ret;
    std::vector<ptr<ASubRule>> leftRet = (*m_son)[0]->getEquivalentRules();
    leftRet.push_back((*m_son)[0]);
    std::vector<ptr<ASubRule>> rightRet = (*m_son)[1]->getEquivalentRules();
    rightRet.push_back((*m_son)[1]);

    for (const auto& subRuleLeft : leftRet)
    {
        for (const auto& subRuleRight : rightRet)
        {
            if constexpr (!std::is_same_v<SubPropertyType, Implication<ASubRule>>)
            {
                ret.push_back(std::make_shared<const SubRule>(subRuleRight, subRuleLeft)); //symmetric case
            }            
            ret.push_back(std::make_shared<const SubRule>(subRuleLeft, subRuleRight)); //direct case            
        }
    }

    //remove last one because it is the same as "this" SubRuleFormula
    ret.pop_back();
    return ret;
}

std::vector<ptr<ASubRule>> SubRule<Hyp<ASubRule>>::getEquivalentRules() const
{
    std::vector<ptr<ASubRule>> ret;
    std::vector<std::vector<ptr<ASubRule>>> allEquiSubRules;
    for (size_t k = 0; k < m_son->arity(); k++)
    {
        auto equiSubRules_k = (*m_son)[k]->getEquivalentRules();
        equiSubRules_k.push_back((*m_son)[k]);
        allEquiSubRules.push_back(equiSubRules_k);
    }
    //init case
    std::vector<std::vector<ptr<ASubRule>>> acc;
    for (size_t k = 0; k < allEquiSubRules[0].size(); k++)
    {
        acc.push_back({ allEquiSubRules[0][k] });
    }
    
    //iterative cartesian product
    for (size_t k = 1; k < allEquiSubRules.size(); k++)
    {
        std::vector<std::vector<ptr<ASubRule>>> newAcc;
        for (size_t accIdx = 0; accIdx < acc.size(); accIdx++)
        {            
            for (size_t j = 0; j < allEquiSubRules[k].size(); j++)
            {
                std::vector<ptr<ASubRule>> crtNewAcc = acc[accIdx];
                crtNewAcc.push_back(allEquiSubRules[k][j]);
                newAcc.push_back(crtNewAcc);
            }
        }
        acc = newAcc;
    }

    //create equivalent SubRules
    for (size_t k = 0; k < acc.size(); k++)
    {
        //permutation case
        std::vector<std::vector<size_t>> permuteTab = permutation(m_son->arity()-1);
        for (const auto& permuter : permuteTab)
        {
            std::vector<ptr<ASubRule>> crtSons;
            for (size_t i = 0; i < permuter.size(); i++)
            {
                crtSons.push_back(acc[k][permuter[i]]);
            }
            crtSons.push_back(acc[k].back());
            ret.push_back(std::make_shared<const SubRule>(crtSons));
        }

        ret.push_back(std::make_shared<const SubRule>(acc[k])); //direct case
    }

    //remove last one because it is the same as "this" SubRuleFormula
    ret.pop_back();

    return ret;
}

std::vector<ptr<ASubRule>> SubRule<Not<ASubRule>>::getEquivalentRules() const
{
    std::vector<ptr<ASubRule>> ret;
    std::vector<ptr<ASubRule>> subRet = (*m_son)[0]->getEquivalentRules();
    subRet.push_back((*m_son)[0]);

    for (const auto& subRule : subRet)
    {
        ret.push_back(std::make_shared<const SubRule>(subRule));
    }

    //remove last one because it is the same as "this" SubRuleFormula
    ret.pop_back();
    return ret;
}


/**---------------------------------------------------------------
 * SubRule::operator==(ASubTheorem) methods
 * ---------------------------------------------------------------
 */

template<SubRuleProperty SubPropertyType>
bool SubRule<SubPropertyType>::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem<ToTheoremOpe<SubPropertyType>>*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

bool SubRule<Hyp<ASubRule>>::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem<ToTheoremOpe<SubPropertyType>>*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

bool SubRule<Not<ASubRule>>::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem<ToTheoremOpe<SubPropertyType>>*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

bool SubRule<Boolean>::isEqual(const ASubTheorem &prop) const
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

bool SubRule<ConstBoolean>::isEqual(const ASubTheorem &prop) const
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


/**---------------------------------------------------------------
 * SubRule::operator==(ASubRule) methods
 * ---------------------------------------------------------------
 */
template<SubRuleProperty SubPropertyType>
bool SubRule<SubPropertyType>::isEqual(const ASubRule &prop) const
{
    auto propCast=dynamic_cast<const SubRule*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

bool SubRule<Hyp<ASubRule>>::isEqual(const ASubRule &prop) const
{
    auto propCast=dynamic_cast<const SubRule*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

bool SubRule<Not<ASubRule>>::isEqual(const ASubRule &prop) const
{
    auto propCast=dynamic_cast<const SubRule*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

bool SubRule<Boolean>::isEqual(const ASubRule &prop) const
{
    auto propCast=dynamic_cast<const SubRule*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

bool SubRule<ConstBoolean>::isEqual(const ASubRule &prop) const
{
    auto propCast=dynamic_cast<const SubRule*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}


/**---------------------------------------------------------------
 * SubTheorem::operator==(ASubRule) methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
bool SubTheorem<SubPropertyType>::isEqual(const ASubRule& prop) const
{
    auto propCast=dynamic_cast<const SubRule<ToRuleOpe<SubPropertyType>>*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

bool SubTheorem<Hyp<ASubTheorem>>::isEqual(const ASubRule& prop) const
{
    auto propCast=dynamic_cast<const SubRule<ToRuleOpe<SubPropertyType>>*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

bool SubTheorem<Not<ASubTheorem>>::isEqual(const ASubRule& prop) const
{
    auto propCast=dynamic_cast<const SubRule<ToRuleOpe<SubPropertyType>>*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

bool SubTheorem<Boolean>::isEqual(const ASubRule& prop) const
{
    auto propCast=dynamic_cast<const SubRule<Boolean>*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

bool SubTheorem<ConstBoolean>::isEqual(const ASubRule& prop) const
{
    auto propCast=dynamic_cast<const SubRule<SubPropertyType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}


/**---------------------------------------------------------------
 * SubRule::operator==(SubRule) methods
 * ---------------------------------------------------------------
 */
template<SubRuleProperty SubPropertyType>
bool SubRule<SubPropertyType>::operator==(const SubRule &prop) const
{
    return *m_son==prop.getSon();
}


/**---------------------------------------------------------------
 * SubRule::operator==(SubTheorem) methods
 * ---------------------------------------------------------------
 */
template<SubRuleProperty SubPropertyType>
bool SubRule<SubPropertyType>::operator==(const SubTheorem<ToTheoremOpe<SubPropertyType>> &prop) const
{
    return *m_son==prop.getSon();
}


/**---------------------------------------------------------------
 * SubTheorem::operator==(SubRule) methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
bool SubTheorem<SubPropertyType>::operator==(const SubRule<ToRuleOpe<SubPropertyType>>& prop) const
{
    return *m_son==(prop.getSon());
}



/**----------------------------------------------------------------
 *  Equal operator case
 * ----------------------------------------------------------------
 */
template<typename ValueType1, typename ValueType2>
bool SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
identifyPriv(const ptr<ASubTheorem> &prop, DbVarProp &dbVarProp) const
{
    if(prop->type()==PropType::EQUAL_PROP)
    {
        auto propCast=std::static_pointer_cast<const SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>,
                ASubArithmeticTheorem<ValueType2> >>>(prop);
        return get<0>(*m_son)->identifyPriv(get<0>(propCast->getSon()),dbVarProp) &&
                get<1>(*m_son)->identifyPriv(get<1>(propCast->getSon()),dbVarProp);
    }
    return false;
}

template<typename ValueType1, typename ValueType2>
ptr<ASubTheorem> SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
applyPriv(DbVarProp &dbVarProp) const
{
    return Logic::make_theorem_formula<SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >>>(
    get<0>(*m_son)->applyPriv(dbVarProp),get<1>(*m_son)->applyPriv(dbVarProp));
}

template<typename ValueType1, typename ValueType2>
ptr<ASubTheorem> SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
applyFirstPriv(DbVarProp &dbVarProp) const
{
    return Logic::make_theorem_formula<Theorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >>>(
    get<0>(*m_son)->applyPriv(dbVarProp),get<1>(*m_son)->applyPriv(dbVarProp));
}

template<typename ValueType1, typename ValueType2>
ptr<ASubTheorem> SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >>>(
        logicIdx, get<0>(*m_son)->applyPriv(dbVarProp, logicIdx), get<1>(*m_son)->applyPriv(dbVarProp, logicIdx));
}

template<typename ValueType1, typename ValueType2>
ptr<ASubTheorem> SubRule<Equal<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> > >::
applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<Theorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >>>(
        logicIdx, get<0>(*m_son)->applyPriv(dbVarProp, logicIdx), get<1>(*m_son)->applyPriv(dbVarProp, logicIdx));
}

/**----------------------------------------------------------------
 *  BelongsTo operator case
 * ----------------------------------------------------------------
 */
template<typename SetType>
bool SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>> >::
identifyPriv(const ptr<ASubTheorem> &prop, DbVarProp &dbVarProp) const
{
    if(prop->type()==PropType::EQUAL_PROP)
    {
        auto propCast=std::dynamic_pointer_cast<const SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,
                ASubArithmeticTheorem<SetType>>>>(prop);
        return get<0>(*m_son)->identifyPriv(get<0>(propCast->getSon()),dbVarProp) &&
                get<1>(*m_son)->identifyPriv(get<1>(propCast->getSon()),dbVarProp);
    }
    return false;
}

template<typename SetType>
ptr<ASubTheorem> SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>, ASubArithmeticRule<SetType> > >::
applyPriv(DbVarProp &dbVarProp) const
{
    return Logic::make_theorem_formula<SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>>>
            (get<0>(*m_son)->applyPriv(dbVarProp),get<1>(*m_son)->applyPriv(dbVarProp));
}

template<typename SetType>
ptr<ASubTheorem> SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>> >::
applyFirstPriv(DbVarProp &dbVarProp) const
{
    return Logic::make_theorem_formula<Theorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>>>
                                    (get<0>(*m_son)->applyPriv(dbVarProp),get<1>(*m_son)->applyPriv(dbVarProp));
}

template<typename SetType>
ptr<ASubTheorem> SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>, ASubArithmeticRule<SetType> > >::
applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType>>>>
        (logicIdx, get<0>(*m_son)->applyPriv(dbVarProp, logicIdx), get<1>(*m_son)->applyPriv(dbVarProp, logicIdx));
}

template<typename SetType>
ptr<ASubTheorem> SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>, ASubArithmeticRule<SetType>> >::
applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<Theorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType>>>>
        (logicIdx, get<0>(*m_son)->applyPriv(dbVarProp, logicIdx), get<1>(*m_son)->applyPriv(dbVarProp, logicIdx));
}

/**----------------------------------------------------------------
 *  Let BelongsTo operator case
 * ----------------------------------------------------------------
 */
template<>
bool SubRule<Let<ASubArithmeticRule<void>, ASubRule > >::
identifyPriv(const ptr<ASubTheorem> &prop, DbVarProp &dbVarProp) const
{
    auto propCast=std::dynamic_pointer_cast<const SubTheorem<Let<ASubArithmeticTheorem<void>,
            ASubTheorem>>>(prop);
    if(propCast)
    {
        return get<0>(*m_son)->identifyPriv(get<0>(propCast->getSon()),dbVarProp) &&
                get<1>(*m_son)->identifyPriv(get<1>(propCast->getSon()),dbVarProp);
    }
    return false;
}

template<>
ptr<ASubTheorem> SubRule<Let<ASubArithmeticRule<void>, ASubRule > >::
applyPriv(DbVarProp &dbVarProp) const
{
    return Logic::make_theorem_formula<SubTheorem<Let<ASubArithmeticTheorem<void>, ASubTheorem> >>(
    get<0>(*m_son)->applyPriv(dbVarProp),get<1>(*m_son)->applyPriv(dbVarProp));
}

template<>
ptr<ASubTheorem> SubRule<Let<ASubArithmeticRule<void>, ASubRule > >::
applyFirstPriv(DbVarProp &dbVarProp) const
{
    return Logic::make_theorem_formula<Theorem<Let<ASubArithmeticTheorem<void>, ASubTheorem>>>(
    get<0>(*m_son)->applyPriv(dbVarProp),get<1>(*m_son)->applyPriv(dbVarProp));
}

template<>
ptr<ASubTheorem> SubRule<Let<ASubArithmeticRule<void>, ASubRule > >::
applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<SubTheorem<Let<ASubArithmeticTheorem<void>, ASubTheorem> >>(
        logicIdx, get<0>(*m_son)->applyPriv(dbVarProp, logicIdx), get<1>(*m_son)->applyPriv(dbVarProp, logicIdx));
}

template<>
ptr<ASubTheorem> SubRule<Let<ASubArithmeticRule<void>, ASubRule > >::
applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<Theorem<Let<ASubArithmeticTheorem<void>, ASubTheorem>>>(
        logicIdx, get<0>(*m_son)->applyPriv(dbVarProp, logicIdx), get<1>(*m_son)->applyPriv(dbVarProp, logicIdx));
}


/**----------------------------------------------------------------
 *  Let SetEqual operator case
 * ----------------------------------------------------------------
 */

template<>
bool SubRule<Let<ASubRule, ASubRule > >::
identifyPriv(const ptr<ASubTheorem> &prop, DbVarProp &dbVarProp) const
{
    auto propCast=std::dynamic_pointer_cast<const SubTheorem<Let<ASubTheorem,
            ASubTheorem>>>(prop);
    if(propCast)
    {
        return get<0>(*m_son)->identifyPriv(get<0>(propCast->getSon()),dbVarProp) &&
                get<1>(*m_son)->identifyPriv(get<1>(propCast->getSon()),dbVarProp);
    }
    return false;
}

template<>
ptr<ASubTheorem> SubRule<Let<ASubRule, ASubRule > >::
applyPriv(DbVarProp &dbVarProp) const
{
    return Logic::make_theorem_formula<SubTheorem<Let<ASubTheorem, ASubTheorem> >>(
    get<0>(*m_son)->applyPriv(dbVarProp),get<1>(*m_son)->applyPriv(dbVarProp));
}

template<>
ptr<ASubTheorem> SubRule<Let<ASubRule, ASubRule > >::
applyFirstPriv(DbVarProp &dbVarProp) const
{
    return Logic::make_theorem_formula<Theorem<Let<ASubTheorem, ASubTheorem>>>(
    get<0>(*m_son)->applyPriv(dbVarProp),get<1>(*m_son)->applyPriv(dbVarProp));
}

template<>
ptr<ASubTheorem> SubRule<Let<ASubRule, ASubRule > >::
applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<SubTheorem<Let<ASubTheorem, ASubTheorem> >>(
        logicIdx, get<0>(*m_son)->applyPriv(dbVarProp, logicIdx), get<1>(*m_son)->applyPriv(dbVarProp, logicIdx));
}

template<>
ptr<ASubTheorem> SubRule<Let<ASubRule, ASubRule > >::
applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const
{
    return Logic::make_theorem_formula<Theorem<Let<ASubTheorem, ASubTheorem>>>(
        logicIdx, get<0>(*m_son)->applyPriv(dbVarProp, logicIdx), get<1>(*m_son)->applyPriv(dbVarProp, logicIdx));
}
