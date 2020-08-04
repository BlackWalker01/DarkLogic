/*===--- axiom.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements axiom class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "axiom.h"

using namespace N_DarkLogic;

ptr<ASubRule> N_DarkLogic::createAxiom(const std::string &name, const std::string &content)
{
    //parse hypothesis
    char c='\0';

    //look for operators and variables
    size_t numPar = 0;
    size_t nbBraceBracket = 0;
    Name crtNameOpe = NONE;
    std::string crtVar = "";
    std::vector<OperatorOrdering> opeList;
    //std::vector<unsigned short> parLvlList; //parenthesis level list
    std::vector<OperatorOrdering> hyps;
    std::vector<HypParams> hypStack;
    std::vector<std::shared_ptr<VariableContainer>> varList;
    DbVarContainer dbVar;
    for (size_t k = 0; k < content.size(); k++)
    {
        c = content[k];
        switch (c)
        {
        case '(':
        {
            numPar++;
            continue;
        }
        case ')':
        {
            if (numPar == 0)
            {
                throw std::runtime_error("Unexpected ')' character at " + sizeToString(k) + " index");
            }
            // close all hypothesis operators in that parenthesis
            while (hyps.size() && hyps.back().nbPar == numPar)
            {
                hyps.pop_back();
                hypStack.pop_back();
            }
            numPar--;
            continue;
        }
        case ' ':
        {
            continue;
        }
        //hypothesis cases
        case '{':
        {
            OperatorOrdering opeOrdering;
            opeOrdering.nbPar = numPar;
            opeOrdering.hyps = hypStack;
            opeOrdering.hyps = hypStack;
            hyps.push_back(opeOrdering);
            hypStack.push_back(HypParams(opeList.size(),0));
            nbBraceBracket++;
            continue;
        }
        case ',':
        {
            if (hyps.size())
            {
                // close latest hypothesis operators if necessary
                while (nbBraceBracket < hyps.size() && hyps[hyps.size() - 2].nbPar == hyps[hyps.size() - 1].nbPar
                    && hyps[hyps.size() - 1].foundCcl)
                {
                    hyps.pop_back();
                    hypStack.pop_back();
                }
                hyps.back().nbArgs++;
                hypStack.back().argIdx++;
            }
            else
            {
                throw std::runtime_error("unexpected ',' character at " + sizeToString(k) + " index");
            }
            continue;
        }
        case '}':
        {
            nbBraceBracket--;
            // close latest hypothesis operators if necessary
            while (nbBraceBracket + 2 <= hyps.size() && hyps[hyps.size() - 2].nbPar == hyps[hyps.size() - 1].nbPar
                && hyps[hyps.size() - 1].foundCcl)
            {
                hyps.pop_back();
                hypStack.pop_back();
            }
            if (hyps.size() && numPar == hyps.back().nbPar && hypStack.size())
            {
                hyps.back().nbArgs++;
                hyps.back().nbArgs++; //increment to add implication of hypothesis operator in its arity
                hypStack.back().argIdx++;
                auto it = opeList.begin() + static_cast<long long>(hypStack.back().idx);
                hyps.back().ope = createRuleOperator(HYP, hyps.back().nbArgs);
                hyps.back().foundCcl = true;
                opeList.insert(it, hyps.back());
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
        IOperator::skipStrings(content, k);
        size_t crtK = k;
        if ((crtNameOpe = IOperator::getNextOpeName(content, k)))
        {
            addRuleOperator(crtNameOpe, opeList, hyps, hypStack, numPar);
            crtNameOpe = NONE;
            k--;
        }
        else
        {
            crtVar = getVarName(content, k);
            if (crtVar != "")
            {
                ASubRule::addAbstractTerm(crtVar, varList, dbVar);
                crtVar = "";
                k--;
            }
            else if (k == crtK)
            {
                std::string ch = "";
                ch += c;
                throw std::runtime_error("Unexpected '" + ch + "' character at " + sizeToString(k) + " index");
            }
        }
    }

    //order operators
    std::vector<OperatorOrdering> orderedOpeList=orderOperator(opeList);
    return createAxiom(name,opeList,orderedOpeList,varList);
}

ptr<ASubRule> N_DarkLogic::createAxiom(const std::string &name, std::vector<OperatorOrdering> &opeList,
                                       std::vector<OperatorOrdering> &orderedOpeList,
                                       std::vector<std::shared_ptr<VariableContainer> > &varList)
{
    if(orderedOpeList.size())
    {
        auto ope=orderedOpeList[0].ope;
        switch(ope->arity())
        {
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
                                        return std::make_shared<const Axiom<Equivalent<ASubRule>>>
                                        (name,leftOpeCast,rightOpeCast);
                                    }
                                    default:
                                    {
                                        throw std::runtime_error("Equivalent operator expects boolean variable");
                                    }
                                }
                            }
                            default:
                            {
                                throw std::runtime_error("Equivalent operator expects boolean variable");
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
                                        return std::make_shared<const Axiom<Implication<ASubRule >>>
                                        (name,leftOpeCast,rightOpeCast);
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
                    default:
                    {
                        throw std::runtime_error("First operator of a rule must be implication or equivalent");
                    }
                }
            }

            default:
            {
                throw std::runtime_error("First operator of a rule must be implication or equivalent");
            }
        }
    }
    else
    {
        throw std::runtime_error("First operator of a rule must be implication or equivalent");
    }
    return nullptr;
}
