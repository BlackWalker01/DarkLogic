#include "subtheorem.h"
#include "Utils/utils.h"
#include "Variable/allvariable.h"
#include "Formula/Arithmetic/VariableFormula/variabletheoremformula.h"

using namespace N_DarkLogic;

const std::vector<std::vector<Arity> > &ASubTheorem::getAllPaths() const
{
    return m_allPaths;
}

const std::vector<std::vector<Arity>>& N_DarkLogic::ASubTheorem::getImplPaths() const
{
    return m_implPaths;
}

bool N_DarkLogic::ASubTheorem::isEvaluated() const
{
    throw std::logic_error("SubTheorem cannot call isEvaluated method");
}

/**
 * Default implementation of canBeDemonstrated method
 * return true if for all values of its variables, the theorem is evaluate to true (or false) 
 */
bool N_DarkLogic::ASubTheorem::canBeDemonstrated() const
{
    std::unique_ptr<bool> eval = nullptr;
    return testCanBeDemonstrated(getExtVars()->getVars(), eval);    
}

std::vector<std::pair<Evaluater::ConfigEval, bool>> N_DarkLogic::ASubTheorem::getConfigEvals() const
{
    return std::vector<std::pair<Evaluater::ConfigEval, bool>>();
}

std::unordered_map<IDVar,IDVar> N_DarkLogic::ASubTheorem::getVarToEval() const
{
    return std::unordered_map<IDVar, IDVar>();
}

std::vector<Evaluater::ConfigEval> ASubTheorem::getCompatibleConfigs(const Evaluater::ConfigEval& commonConfig,
    const std::unordered_map<IDVar,IDVar>& internalVars) const
{
    return std::vector<Evaluater::ConfigEval>();
}

/**
 * return true if the theorem has one crtValue
 * crtValue is the current evaluation of the theorem
*/
bool N_DarkLogic::ASubTheorem::testCanBeDemonstrated(const std::vector<ptr<AVariable>>& vars,
    std::unique_ptr<bool>& crtValue) const
{
    //try to evaluate directly 
    try
    {
        if (crtValue)
        {
            bool newValue = getHiddenValue();
            return (*crtValue) == newValue;
        }
        else
        {
            crtValue = std::make_unique<bool>(getHiddenValue());
            return true;
        }    
    }
    //else simulate with all possible values for variables
    catch (const VariableException&)
    {
        bool ret = true;
        for (size_t k = 0; k < vars.size(); k++)
        {
            auto var = vars[k];
            auto remainVars = vars;
            remainVars.erase(remainVars.begin() + k);

            //if var is not Boolean continue
            if (var->valueType() != VALUE_TYPE::BOOL_TYPE)
            {
                continue;
            }

            auto boolVar = std::static_pointer_cast<const Boolean>(var);

            //try to evaluate theorem with var=true
            (*boolVar) = (true);
            ret = testCanBeDemonstrated(remainVars, crtValue);
            if (!ret)
            {
                boolVar->unset();
                return false;
            }


            //try to evaluate theorem with var= false
            (*boolVar) = (false);
            ret = testCanBeDemonstrated(remainVars, crtValue);
            if (!ret)
            {
                boolVar->unset();
                return false;
            }

            //unset var
            boolVar->unset();
        }
        return ret;
    }
}


ptr<IOperator> N_DarkLogic::createTheoremOperator(const Name name, const N_DarkLogic::Arity &arity)
{
    switch (name)
    {
        case NONE:
        {
            throw std::runtime_error("Unknown operator");
        }
        case AND:
        {
            return std::make_shared<const And<ASubTheorem>>();
        }
        case EQUIVALENT:
        {
            return std::make_shared<const Equivalent<ASubTheorem>>();
        }
        case IMPLICATION:
        {
            return std::make_shared<const Implication<ASubTheorem>>();
        }
        case NOT:
        {
            return std::make_shared<const Not<ASubTheorem>>();
        }
        case OR:
        {
            return std::make_shared<const Or<ASubTheorem>>();
        }
        case HYP:
        {
            std::vector<ptr<ASubTheorem>> props;
            for(size_t k=0;k<arity;k++)
            {
                props.push_back(nullptr);
            }
            return std::make_shared<const Hyp<ASubTheorem>>(props);
        }
        case SETEQUAL:
        {
            return std::make_shared<const SetEqual<SubArithmeticTheorem<NaturalInteger>>>();
        }
        case EQUAL:
        {
            return std::make_shared<const Equal<ASubArithmeticTheorem<size_t>,ASubArithmeticTheorem<size_t>>>();
        }
        case LET:
        {
            return std::make_shared<const Let<SubArithmeticTheorem<NaturalInteger>,ASubTheorem>>();
        }
        case BELONGSTO:
        {
            return std::make_shared<const BelongsTo<ASubArithmeticRule<size_t>,ASubArithmeticRule<NaturalIntegerSet>>>();
        }
        case PLUS:
        {
            return std::make_shared<const Plus<ASubArithmeticTheorem<size_t>,ASubArithmeticTheorem<size_t>>>();
        }
    }
    return nullptr;
}


void N_DarkLogic::addTheoremOperator(const Name opeName, std::vector<OperatorOrdering> &opeList,
                                    const std::vector<OperatorOrdering> &hyps, const std::vector<HypParams>& hypStack,
                                    const size_t &numPar, const std::string &name, size_t &index)
{
    ptr<IOperator> ope=nullptr;
    if((ope=createTheoremOperator(opeName)))
    {
        opeList.push_back(OperatorOrdering(ope, numPar, hypStack));
    }
    else
    {
        throw std::runtime_error("Unknown operator in "+name+" theorem at index="+sizeToString(index)+". Exit...");
    }
}


/**
 * @brief Property::parseContent
 * @param content
 * Ex: content="a&&b=>!c||d" -> (a&&b)=>((!c)||d)
 */
/*
ASubTheorem *N_LogicGame::create(const std::string &name, const std::string &content)
{
    //parse hypothesis
    unsigned int j=0;
    char c='\0';

    //look for operators and variables
    size_t numPar=0;
    std::vector<ParenthesisParam> parenthesisParams; //first is parenthesis level of hypothesis operator, second is the current number of argument
    parenthesisParams.push_back(ParenthesisParam());
    std::string crtNameOpe="";
    IOperator* ope=nullptr;
    std::string crtVar="";
    std::vector<OperatorOrdering> opeList;
    //std::vector<unsigned short> parLvlList; //parenthesis level list
    std::vector<AbstractTerm*> varList;
    DbVar<BooleanSet> dbVar;
    for(unsigned int k=j;k<content.size();k++)
    {
        c=content[k];
        switch(c)
        {
            case '(':
            {
                if(crtVar!="")
                {
                    ASubTheorem::addAbstractTerm(crtVar,varList,dbVar);
                }
                crtVar="";
                if(crtNameOpe!="")
                {
                    if((ope=createTheoremOperator(crtNameOpe)))
                    {
                        opeList.push_back(OperatorOrdering(ope,numPar,parenthesisParams.back().nbArgs));
                        ope=nullptr;
                    }
                    else
                    {
                        throw std::runtime_error("Unknown operator '"+crtNameOpe+"'. Exit...");
                    }
                    crtNameOpe="";
                }
                numPar++;
                parenthesisParams.push_back(ParenthesisParam(numPar,0,opeList.size()));
                continue;
            }
            case ')':
            {

                if(crtVar!="")
                {
                    ASubTheorem::addAbstractTerm(crtVar,varList,dbVar);
                }
                crtVar="";
                if(crtNameOpe!="")
                {
                    if((ope=createTheoremOperator(crtNameOpe)))
                    {
                        opeList.push_back(OperatorOrdering(ope,numPar,parenthesisParams.back().nbArgs));
                        ope=nullptr;
                    }
                    else
                    {
                        throw std::runtime_error("Unknown operator '"+crtNameOpe+"'. Exit...");
                    }
                    crtNameOpe="";
                }
                numPar--;
                parenthesisParams.pop_back();
                continue;
            }
            case ' ':
            {
                if(crtVar!="")
                {
                    ASubTheorem::addAbstractTerm(crtVar,varList,dbVar);
                }
                crtVar="";
                if(crtNameOpe!="")
                {
                    if((ope=createTheoremOperator(crtNameOpe)))
                    {
                        opeList.push_back(OperatorOrdering(ope,numPar,parenthesisParams.back().nbArgs));
                        ope=nullptr;
                    }
                    else
                    {
                        throw std::runtime_error("Unknown operator '"+crtNameOpe+"'. Exit...");
                    }
                    crtNameOpe="";
                }
                continue;
            }
            //hypothesis cases
            case '{':
            {
                if(crtVar!="")
                {
                    throw std::runtime_error("Unexpected hypothesis operator just after a variable. Exit...");
                }
                if(crtNameOpe!="")
                {
                    if((ope=createTheoremOperator(crtNameOpe)))
                    {
                        opeList.push_back(OperatorOrdering(ope,numPar,parenthesisParams.back().nbArgs));
                        ope=nullptr;
                    }
                    else
                    {
                        throw std::runtime_error("Unknown operator '"+crtNameOpe+"'. Exit...");
                    }
                    crtNameOpe="";
                }
                numPar++;
                parenthesisParams.push_back(ParenthesisParam(numPar,0,opeList.size()));
                continue;
            }
            case ',':
            {
                if(numPar==parenthesisParams.back().nbPar)
                {
                    parenthesisParams.back().nbArgs++;
                    if(crtVar!="")
                    {
                        ASubTheorem::addAbstractTerm(crtVar,varList,dbVar);
                    }
                    crtVar="";

                }
                else
                {
                    throw std::runtime_error("Syntax error in the last function. Exit...");
                }
                continue;
            }
            case '}':
            {
                if(numPar==parenthesisParams.back().nbPar)
                {
                    if(crtVar!="")
                    {
                        ASubTheorem::addAbstractTerm(crtVar,varList,dbVar);
                        parenthesisParams.back().nbArgs++;
                    }
                    crtVar="";
                    if(numPar==parenthesisParams.back().nbPar)
                    {
                        if(parenthesisParams.back().nbArgs>0)
                        {
                            parenthesisParams.back().nbArgs++; //increment to add implication of hypothesis operator in its arity
                            auto it=opeList.begin()+static_cast<long long>(parenthesisParams.back().indexInOpeList);
                            opeList.insert(it,OperatorOrdering(new Hyp<ASubTheorem>(parenthesisParams.back().nbArgs),numPar,parenthesisParams.back().nbArgs));
                        }
                        parenthesisParams.pop_back();
                    }
                }
                else
                {
                    throw std::runtime_error("Syntax error in hypothesis operator. Exit...");
                }
                numPar--;
                continue;
            }
            default:
            break;
        }
        if(isCharInVar(c))
        {
            crtVar+=c;
            if(crtNameOpe!="")
            {
                if((ope=createTheoremOperator(crtNameOpe)))
                {
                    opeList.push_back(OperatorOrdering(ope,numPar,parenthesisParams.back().nbArgs));
                    ope=nullptr;
                }
                else
                {
                    throw std::runtime_error("Unknown operator '"+crtNameOpe+"'. Exit...");
                }
                crtNameOpe="";
            }
        }
        else
        {
            crtNameOpe+=c;
            if(IOperator::isOpeName(crtNameOpe.substr(0,crtNameOpe.size()-1)) && !IOperator::isOpeName(crtNameOpe))
            {
                ope=createTheoremOperator(crtNameOpe.substr(0,crtNameOpe.size()-1));
                opeList.push_back(OperatorOrdering(ope,numPar,parenthesisParams.back().nbArgs));
                ope=nullptr;
                crtNameOpe=c;
            }
            if(crtVar!="")
            {
                ASubTheorem::addAbstractTerm(crtVar,varList,dbVar);
            }
            crtVar="";
        }
        if(k==content.size()-1)
        {
            if(crtVar!="")
            {
                ASubTheorem::addAbstractTerm(crtVar,varList,dbVar);
            }
            crtVar="";
            if(crtNameOpe!="")
            {
                if((ope=createTheoremOperator(crtNameOpe)))
                {
                    opeList.push_back(OperatorOrdering(ope,numPar,parenthesisParams.back().nbArgs));
                    ope=nullptr;
                }
                else
                {
                    throw std::runtime_error("Unknown operator '"+crtNameOpe+"'. Exit...");
                }
                crtNameOpe="";
            }
        }
    }

    //order operators
    std::vector<OperatorOrdering> orderedOpeList=orderOperator(opeList);
    std::vector<IOperator*> opeList_;
    std::vector<IOperator*> orderedOpeList_;
    for(unsigned int k=0;k<opeList.size();k++)
    {
        opeList_.push_back(opeList[k].ope);
        orderedOpeList_.push_back(orderedOpeList[k].ope);
    }
    if(orderedOpeList_.size()>0)
    {
        switch(orderedOpeList_[0]->name())
        {
            case AND:
            {
                return new SubTheorem<And<ASubTheorem>>(name,opeList_,orderedOpeList_,varList);
            }
            case EQUIVALENT:
            {
                return new SubTheorem<Equivalent<ASubTheorem>>(name,opeList_,orderedOpeList_,varList);
            }
            case HYP:
            {
                return new SubTheorem<Hyp<ASubTheorem>>(name,opeList_,orderedOpeList_,varList);
            }
            case IMPLICATION:
            {
                return new SubTheorem<Implication<ASubTheorem>>(name,opeList_,orderedOpeList_,varList);
            }
            case NOT:
            {
                return new SubTheorem<Not<ASubTheorem>>(name,opeList_,orderedOpeList_,varList);
            }
            case OR:
            {
                return new SubTheorem<Or<ASubTheorem>>(name,opeList_,orderedOpeList_,varList);
            }
        }
    }
    else if (varList.size()==1)
    {
        if(varList[0]->name()=="true")
        {
            return new SubTheorem<ConstBoolean>(name,opeList_,orderedOpeList_,varList);
        }
        else if(varList[0]->name()=="false")
        {
            return new SubTheorem<ConstBoolean>(name,opeList_,orderedOpeList_,varList);
        }
        else
        {
            return new SubTheorem<Boolean>(name,opeList_,orderedOpeList_,varList);
        }
    }
    throw std::runtime_error("Syntax error in property. Exit...");
}*/

