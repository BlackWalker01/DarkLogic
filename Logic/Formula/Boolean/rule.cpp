#include "rule.h"
#include "Operator/operator.h"
#include "Formula/dbvarprop.h"
//#include "logichash.h"
#include "Variable/Boolean/boolean.h"
#include "Variable/dbvar.h"
#include "theorem.h"
#include "Formula/Arithmetic/allarithformula.h"

using namespace N_Logic;

void ASubRule::addAbstractTerm(const std::string &varName, std::vector<std::shared_ptr<VariableContainer>> &varList, N_Logic::DbVarContainer &dbVar)
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
        bool isConst=true;
        //bool isReal=false;
        for(size_t k=0;k<var->name().size();k++)
        {
            char c=var->name()[k];
            if(c<'0' || c>'9')
            {
                isConst=false;
                break;
            }
            if(c=='.')
            {
                //isReal=true;
            }
        }
        if(!isConst && var->name()!="true" && var->name()!="false")
        {
            dbVar.insert(varContainer);
        }
    }
    varList.push_back(varContainer);
}

ptr<ASubRule> N_Logic::createRule(const std::string &name, const std::string &content)
{
    //parse hypothesis
    char c='\0';

    //look for operators and variables
    size_t numPar=0;
    std::vector<ParenthesisParam> parenthesisParams; //first is parenthesis level of hypothesis operator, second is the current number of argument
    parenthesisParams.push_back(ParenthesisParam());
    Name crtNameOpe=NONE;
    std::string crtVar="";
    std::vector<OperatorOrdering> opeList;
    //std::vector<unsigned short> parLvlList; //parenthesis level list
    std::vector<OperatorOrdering> hyps;
    std::vector<std::shared_ptr<VariableContainer>> varList;
    DbVarContainer dbVar;
    for (size_t k = 0; k < content.size(); k++)
    {
        c = content[k];
        switch (c)
        {
        case '(':
        {
            parenthesisParams.push_back(ParenthesisParam(numPar, opeList.size()));
            numPar++;
            continue;
        }
        case ')':
        {
            if (hyps.size() && hyps.back().nbPar == numPar)
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
            parenthesisParams.push_back(ParenthesisParam(numPar, opeList.size()));
            numPar++;
            OperatorOrdering opeOrdering;
            opeOrdering.nbPar = parenthesisParams.back().nbPar;
            hyps.push_back(opeOrdering);
            continue;
        }
        case ',':
        {
            if (hyps.size())
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
            if ((numPar == parenthesisParams.back().nbPar + 1) && hyps.size())
            {
                hyps.back().nbArgs++;
                if (hyps.back().nbArgs > 0)
                {
                    hyps.back().nbArgs++; //increment to add implication of hypothesis operator in its arity
                    auto it = opeList.begin() + static_cast<long long>(parenthesisParams.back().indexInOpeList);
                    hyps.back().ope = createRuleOperator(HYP, hyps.back().nbArgs);
                    /*OperatorOrdering opeOrdering(createRuleOperator(HYP,hyps.back().nbArgs),
                                                 parenthesisParams.back().nbPar,hyps[parenthesisParams.size()-2].nbArgs);*/
                    opeList.insert(it, hyps.back());
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
        IOperator::skipStrings(content, k);
        size_t crtK = k;
        if ((crtNameOpe = IOperator::getNextOpeName(content, k)))
        {
            addRuleOperator(crtNameOpe, opeList, hyps, numPar);
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
    return createRule(name,opeList,orderedOpeList,varList);
}

ptr<ASubRule> N_Logic::createRule(const std::string &name, std::vector<OperatorOrdering> &opeList,
                                          std::vector<OperatorOrdering> &orderedOpeList,
                                      std::vector<std::shared_ptr<VariableContainer>> &varList)
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
                                        return std::make_shared<const Rule<Equivalent<ASubRule>>>
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
                                        return std::make_shared<const Rule<Implication<ASubRule >>>
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

template<>
inline bool Rule<Equivalent<ASubRule>>::isSymetric() const
{
    return true;
}