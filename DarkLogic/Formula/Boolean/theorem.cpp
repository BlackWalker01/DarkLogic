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

void ASubTheorem::addAbstractTerm(const std::string &varName, std::vector<std::shared_ptr<VariableContainer>> &varList,
                                  DbVarContainer &dbVar)
{
    std::shared_ptr<VariableContainer> varContainer=nullptr;
    std::shared_ptr<AbstractTerm> var=nullptr;
    if(dbVar.contains(varName))
    {
        varContainer=dbVar.find(varName);
    }
    else
    {
        var=AbstractTerm::create(varName);
        varContainer=std::make_shared<VariableContainer>(var);
        if(!var->isConstant())
        {
            dbVar.insert(varContainer);
        }
    }
    varList.push_back(varContainer);
}

/**
 * @brief Property::parseContent
 * @param content
 * Ex: content="a&&b=>!c||d" -> (a&&b)=>((!c)||d)
 */
ptr<ASubTheorem> N_DarkLogic::createTheorem(const std::string &name, const std::string &content)
{
    //parse hypothesis
    char c='\0';

    //look for operators and variables
    size_t numPar=0;
    std::vector<ParenthesisParam> parenthesisParams;
    parenthesisParams.push_back(ParenthesisParam());
    Name crtNameOpe=NONE;
    std::string crtVar="";
    std::vector<OperatorOrdering> opeList;
    //std::vector<unsigned short> parLvlList; //parenthesis level list
    std::vector<OperatorOrdering> hyps;
    std::vector<std::shared_ptr<VariableContainer>> varList;
    DbVarContainer dbVar;
    for(size_t k=0;k<content.size();k++)
    {
        c=content[k];
        switch(c)
        {
            case '(':
            {
                parenthesisParams.push_back(ParenthesisParam(numPar,opeList.size()));
                numPar++;
                continue;
            }
            case ')':
            {
                if(hyps.size() && hyps.back().nbPar==numPar)
                {
                    hyps.pop_back();
                }
                numPar--;
                parenthesisParams.pop_back();
                continue;
            }
            case ' ':
            {
                continue;
            }
            //hypothesis cases
            case '{':
            {
                parenthesisParams.push_back(ParenthesisParam(numPar,opeList.size()));
                numPar++;
                OperatorOrdering opeOrdering;
                opeOrdering.nbPar = parenthesisParams.back().nbPar;
                hyps.push_back(opeOrdering);
                continue;
            }
            case ',':
            {
                if(hyps.size())
                {
                    hyps.back().nbArgs++;
                }
                else
                {
                    throw std::runtime_error("unexpected ',' character at " + sizeToString(k) + " index");
                }
                continue;
            }
            case '}':
            {
                if((numPar==parenthesisParams.back().nbPar+1) && hyps.size())
                {
                    hyps.back().nbArgs++;
                    if(hyps.back().nbArgs>0)
                    {
                        hyps.back().nbArgs++; //increment to add implication of hypothesis operator in its arity
                        auto it=opeList.begin()+static_cast<long long>(parenthesisParams.back().indexInOpeList);
                        hyps.back().ope = createTheoremOperator(HYP, hyps.back().nbArgs);
                        /*OperatorOrdering opeOrdering(createRuleOperator(HYP,hyps.back().nbArgs),
                                                     parenthesisParams.back().nbPar,hyps[parenthesisParams.size()-2].nbArgs);*/
                        opeList.insert(it,hyps.back());
                    }
                    numPar--;
                    parenthesisParams.pop_back();
                }
                else
                {
                    throw std::runtime_error("Unexpected '}' character at " + sizeToString(k) + " index");
                }
                continue;
            }
            default:
            break;
        }

        //skip strings if it is possible
        IOperator::skipStrings(content,k);
        size_t crtK=k;
        if((crtNameOpe=IOperator::getNextOpeName(content,k)))
        {
            addTheoremOperator(crtNameOpe,opeList,hyps,numPar,name,k);
            crtNameOpe=NONE;
            k--;
        }
        else
        {
            crtVar=getVarName(content,k);
            if(crtVar!="")
            {
                ASubRule::addAbstractTerm(crtVar,varList,dbVar);
                crtVar="";
                k--;
            }
            else if(k==crtK)
            {
                std::string ch="";
                ch+=c;
                throw std::runtime_error("Unexpected '"+ch+"' character at "+sizeToString(k)+" index");
            }
        }
    }

    //order operators
    std::vector<OperatorOrdering> orderedOpeList=orderOperator(opeList);
    return std::static_pointer_cast<const ASubTheorem>(createTheorem(name,opeList,orderedOpeList,varList));
}

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
template<SubTheoremProperty SubPropertyType>
ptr<IISubTheoremFormula> N_DarkLogic::Theorem<SubPropertyType>::ruleApply(const IISubRuleFormula &rule, DbVarProp& dbVarProp, 
    std::vector<Arity> &indexes, const Action::Id& actionKey) const
{
    Arity index=indexes[0];
    indexes.erase(indexes.begin());
    if(index==0)
    {
        return Logic::make_theorem_formula<Theorem<SubPropertyType>>(
            std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey,
                (*this->m_son)[0], dbVarProp, indexes)), (*this->m_son)[1]);
    }
    else
    {
        return Logic::make_theorem_formula<Theorem<SubPropertyType>>(
        (*this->m_son)[0], std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey,
            (*this->m_son)[1], dbVarProp, indexes)));
    }
}

template<SubTheoremProperty SubPropertyType>
ptr<IISubTheoremFormula> N_DarkLogic::Theorem<SubPropertyType>::ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
    std::vector<Arity>& indexes, const Action::Id& actionKey, const size_t& logicIdx) const
{
    Arity index = indexes[0];
    indexes.erase(indexes.begin());
    if (index == 0)
    {
        return Logic::make_theorem_formula<Theorem<SubPropertyType>>(logicIdx,
            std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey,
                (*this->m_son)[0], dbVarProp, indexes, logicIdx)), (*this->m_son)[1]);
    }
    else
    {
        return Logic::make_theorem_formula<Theorem<SubPropertyType>>(logicIdx,
            (*this->m_son)[0], std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey,
                (*this->m_son)[1], dbVarProp, indexes, logicIdx)));
    }
}

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


/**---------------------------------------------------------------
 * createTheorem function
 * ---------------------------------------------------------------
 */
ptr<ValueTypeObject> N_DarkLogic::createTheorem(const std::string &name, std::vector<OperatorOrdering> &opeList,
                                            std::vector<OperatorOrdering> &orderedOpeList,
                                                std::vector<std::shared_ptr<VariableContainer> > &varList)
{
    if(orderedOpeList.size())
    {
        auto ope=orderedOpeList[0].ope;
        if(ope->name()==HYP)
        {
            //size_t nbPar=orderedOpeList[0].nbPar;
            orderedOpeList.erase(orderedOpeList.begin());
            opeList.erase(opeList.begin());
            std::vector<OperatorOrdering> topOpeList, topOrderedOpeList, queueOpeList, queueOrderedOpeList;
            std::vector<ptr<IISubTheoremFormula>> subProps;
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
                subProps.push_back(std::dynamic_pointer_cast<const IISubTheoremFormula>
                                   (createSubTheorem(name+ope->symbol()+sizeToString(k),topOpeList,topOrderedOpeList,varList)));
                topOrderedOpeList.clear();
                queueOrderedOpeList.clear();
            }

            switch (ope->name())
            {
                case HYP:
                {
                    ptr<ASubTheorem> subPropCast=nullptr;
                    std::vector<ptr<ASubTheorem>> subPropCasts;
                    for(auto subProp: subProps)
                    {
                        if((subPropCast=std::dynamic_pointer_cast<const ASubTheorem>(subProp)))
                        {
                            subPropCasts.push_back(subPropCast);
                        }
                        else
                        {
                            throw std::runtime_error("Expected SubTheorem in hypothesis operator");
                        }
                    }
                    return std::make_shared<const Theorem<Hyp<ASubTheorem>>>(subPropCasts);
                }
                default:
                {
                    throw std::runtime_error("Empty Theorem");
                }
            }
        }
        else
        {
            switch (ope->arity())
            {
                case 1:
                {
                    std::vector<OperatorOrdering> queueOpeList(opeList.begin()+1,opeList.end());
                    std::vector<OperatorOrdering> queueOrderedOpeList(orderedOpeList.begin()+1,orderedOpeList.end());
                    switch(ope->name())
                    {
                        case NOT:
                        {
                            auto subOpe=createSubTheorem(name+"!",queueOpeList,queueOrderedOpeList,varList);
                            switch (subOpe->valueType())
                            {
                                case BOOL_TYPE:
                                {
                                    auto subOpeCast=std::static_pointer_cast<const ASubTheorem>(subOpe);
                                    return std::make_shared<const Theorem<Not<ASubTheorem>>>(subOpeCast);
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
                    //separate operators for binary operators
                    std::vector<OperatorOrdering> queueOrderedOpeList(orderedOpeList.begin()+1,orderedOpeList.end());
                    std::vector<OperatorOrdering> leftOpeList;
                    std::vector<OperatorOrdering> rightOpeList;
                    std::vector<OperatorOrdering> leftOrderedOpeList;
                    std::vector<OperatorOrdering> rightOrderedOpeList;
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

                    switch(ope->name())
                    {
                        case AND:
                        {
                            auto leftOpe=createSubTheorem(name+"&&L",leftOpeList,leftOrderedOpeList,varList);
                            auto rightOpe=createSubTheorem(name+"&&R",rightOpeList,rightOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case BOOL_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case BOOL_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubTheorem>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubTheorem>(rightOpe);
                                            return std::make_shared<const Theorem<And<ASubTheorem>>>
                                            (leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("And operator does not handle bool variable");
                                        }
                                    }
                                }
                                default:
                                {
                                    throw std::runtime_error("And operator does not handle bool variable");
                                }
                            }
                        }
                        case EQUIVALENT:
                        {
                            auto leftOpe=createSubTheorem(name+"<=>L",leftOpeList,leftOrderedOpeList,varList);
                            auto rightOpe=createSubTheorem(name+"<=>R",rightOpeList,rightOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case BOOL_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case BOOL_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubTheorem>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubTheorem>(rightOpe);
                                            return std::make_shared<const Theorem<Equivalent<ASubTheorem>>>
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
                            auto leftOpe=createSubTheorem(name+"=>L",leftOpeList,leftOrderedOpeList,varList);
                            auto rightOpe=createSubTheorem(name+"=>R",rightOpeList,rightOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case BOOL_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case BOOL_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubTheorem>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubTheorem>(rightOpe);
                                            return std::make_shared<const Theorem<Implication<ASubTheorem >>>
                                            (leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("Implication operator does not handle bool variable");
                                        }
                                    }
                                }
                                default:
                                {
                                    throw std::runtime_error("Implication operator does not handle bool variable");
                                }
                            }
                        }
                        case OR:
                        {
                            auto leftOpe=createSubTheorem(name+"||L",leftOpeList,leftOrderedOpeList,varList);
                            auto rightOpe=createSubTheorem(name+"||R",rightOpeList,rightOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case BOOL_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case BOOL_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubTheorem>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubTheorem>(rightOpe);
                                            return std::make_shared<const Theorem<Or<ASubTheorem >>>
                                            (leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("Or operator does not handle bool variable");
                                        }
                                    }
                                }
                                default:
                                {
                                    throw std::runtime_error("Or operator does not handle bool variable");
                                }
                            }
                        }

                        case EQUAL:
                        {
                            auto leftOpe=createSubTheorem(name+"==L",leftOpeList,leftOrderedOpeList,varList);
                            auto rightOpe=createSubTheorem(name+"==R",rightOpeList,rightOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case NATURAL_INT_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case NATURAL_INT_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubArithmeticTheorem<size_t>>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubArithmeticTheorem<size_t>>(rightOpe);
                                            return std::make_shared<const Theorem<Equal<ASubArithmeticTheorem<size_t>, ASubArithmeticTheorem<size_t> >>>
                                            (leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("Equal operator does not handle boolean variable");
                                        }
                                    }
                                }
                                default:
                                {
                                    throw std::runtime_error("Equal operator does not handle boolean variable");
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
                                rightOpe=createSubTheorem(name+"€R",rightOpeList,rightOrderedOpeList,varList);
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
                                auto leftOpe=createSubTheorem(name+"€L",leftOpeList,leftOrderedOpeList,varList);
                            }
                            else
                            {
                                leftOpe=createSubTheorem(name+"€L",leftOpeList,leftOrderedOpeList,varList);
                                rightOpe=createSubTheorem(name+"€R",rightOpeList,rightOrderedOpeList,varList);
                            }
                            switch(leftOpe->valueType())
                            {
                                case NATURAL_INT_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case NATURAL_INTEGERSET_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubArithmeticTheorem<size_t>>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubArithmeticTheorem<NaturalIntegerSet>>(rightOpe);
                                            return std::make_shared<const Theorem<BelongsTo<ASubArithmeticTheorem<size_t>,
                                                    ASubArithmeticTheorem<NaturalIntegerSet> >>>(leftOpeCast,rightOpeCast);
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
                            auto leftOpe=createSubTheorem(name+"letL",leftOpeList,leftOrderedOpeList,varList);
                            auto rightOpe=createSubTheorem(name+"letR",rightOpeList,rightOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case VOID_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case BOOL_TYPE:
                                        {
                                            auto leftOpeCast=std::static_pointer_cast<const ASubArithmeticTheorem<void>>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubTheorem>(rightOpe);
                                            return std::make_shared<const Theorem<Let<ASubArithmeticTheorem<void>,ASubTheorem>>>
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
                                            auto leftOpeCast=std::static_pointer_cast<const ASubTheorem>(leftOpe);
                                            auto rightOpeCast=std::static_pointer_cast<const ASubTheorem>(rightOpe);
                                            return std::make_shared<const Theorem<Let<ASubTheorem,ASubTheorem>>>
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
                        default:
                        {
                            throw std::runtime_error("Only boolean operators in top of theorem");
                        }
                    }
                }
                default:
                {
                    throw std::runtime_error("Empty Theorem");
                }
            }
        }
    }
    else if(varList.size())
    {
        auto var=varList[0]->var;
        varList.erase(varList.begin());
        if(var->isConstant())
        {
            switch(var->valueType())
            {
                case VALUE_TYPE::BOOL_TYPE:
                {
                    return std::make_shared<const Theorem<ConstBoolean>>(
                                            std::static_pointer_cast<ConstBoolean>(var)->evaluate());
                }
                default:
                {
                    throw std::runtime_error("Only boolean variable in top of theorem");
                }
            }
        }
        else
        {
            switch(var->valueType())
            {
                case VALUE_TYPE::BOOL_TYPE:
                {
                    return std::make_shared<const Theorem<Boolean>>(
                                            std::static_pointer_cast<Boolean>(var));
                }
                default:
                {
                    throw std::runtime_error("Only boolean variable in top of theorem");
                }
            }
        }
    }
    else
    {
        throw std::runtime_error("Empty Theorem");
    }
    return nullptr;
}
