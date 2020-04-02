#include "Formula/Boolean/allproposition.h"
#include "Formula/Arithmetic/allarithformula.h"

using namespace N_Logic;

ptr<N_Logic::ValueTypeObject> N_Logic::createSubTheorem(const std::string &name, std::vector<OperatorOrdering> &opeList,
                                               std::vector<OperatorOrdering> &orderedOpeList, std::vector<std::shared_ptr<VariableContainer> > &varList)
{
    if(orderedOpeList.size())
    {
        auto ope=orderedOpeList[0].ope;
        if(ope->name()==HYP)
        {
            size_t nbPar=orderedOpeList[0].nbPar;
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
                    return std::make_shared<const SubTheorem<Hyp<ASubTheorem>>>(name+"HYP",subPropCasts);
                }
                default:
                {
                    throw std::runtime_error("Empty SubRule");
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
                                    auto subOpeCast=std::dynamic_pointer_cast<const ASubTheorem>(subOpe);
                                    return std::make_shared<const SubTheorem<Not<ASubTheorem>>>(name,subOpeCast);
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
                                            auto leftOpeCast=std::dynamic_pointer_cast<const ASubTheorem>(leftOpe);
                                            auto rightOpeCast=std::dynamic_pointer_cast<const ASubTheorem>(rightOpe);
                                            return std::make_shared<const SubTheorem<And<ASubTheorem>>>
                                            (name,leftOpeCast,rightOpeCast);
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
                                            auto leftOpeCast=std::dynamic_pointer_cast<const ASubTheorem>(leftOpe);
                                            auto rightOpeCast=std::dynamic_pointer_cast<const ASubTheorem>(rightOpe);
                                            return std::make_shared<const SubTheorem<Equivalent<ASubTheorem>>>
                                            (name,leftOpeCast,rightOpeCast);
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
                                            auto leftOpeCast=std::dynamic_pointer_cast<const ASubTheorem>(leftOpe);
                                            auto rightOpeCast=std::dynamic_pointer_cast<const ASubTheorem>(rightOpe);
                                            return std::make_shared<const SubTheorem<Implication<ASubTheorem >>>
                                            (name,leftOpeCast,rightOpeCast);
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
                                            auto leftOpeCast=std::dynamic_pointer_cast<const ASubTheorem>(leftOpe);
                                            auto rightOpeCast=std::dynamic_pointer_cast<const ASubTheorem>(rightOpe);
                                            return std::make_shared<const SubTheorem<Or<ASubTheorem >>>
                                            (name,leftOpeCast,rightOpeCast);
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
                                            auto leftOpeCast=std::dynamic_pointer_cast<const ASubArithmeticTheorem<size_t>>(leftOpe);
                                            auto rightOpeCast=std::dynamic_pointer_cast<const ASubArithmeticTheorem<size_t>>(rightOpe);
                                            return std::make_shared<const SubTheorem<Equal<ASubArithmeticTheorem<size_t>, ASubArithmeticTheorem<size_t> >>>
                                            (name,leftOpeCast,rightOpeCast);
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
                                leftOpe=createSubTheorem(name+"€L",leftOpeList,leftOrderedOpeList,varList);
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
                                            auto leftOpeCast=std::dynamic_pointer_cast<const ASubArithmeticTheorem<size_t>>(leftOpe);
                                            auto rightOpeCast=std::dynamic_pointer_cast<const ASubArithmeticTheorem<NaturalIntegerSet>>(rightOpe);
                                            return std::make_shared<const SubTheorem<BelongsTo<ASubArithmeticTheorem<size_t>,
                                                    ASubArithmeticTheorem<NaturalIntegerSet> >>>(name,leftOpeCast,rightOpeCast);
                                        }
                                        default:
                                        {
                                            throw std::runtime_error("BelongsTo operator expects a set as right operand");
                                        }
                                    }
                                }
                                default:
                                {
                                    throw std::runtime_error("BelongsTo operator does not handle bool variable");
                                }
                            }
                        }
                        case SETEQUAL:
                        {
                            if(varList.size()<2)
                            {
                                throw std::runtime_error("= operator expects 2 arguments but got only "+sizeToString(varList.size()));
                            }
                            auto firstVar=varList[0];
                            varList.erase(varList.begin());
                            auto rightOpe=createSubTheorem(name+"=R",rightOpeList,rightOrderedOpeList,varList);
                            varList.insert(varList.begin(),firstVar);
                            if(firstVar->type==VALUE_TYPE::VOID_TYPE)
                            {
                                firstVar->type=rightOpe->valueType();
                                firstVar->init();
                            }
                            else if(firstVar->type!=rightOpe->valueType())
                            {
                                throw std::runtime_error(firstVar->var->name()+" is not of the same type of right operand");
                            }
                            auto leftOpe=createSubTheorem(name+"=L",leftOpeList,leftOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case NATURAL_INT_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case NATURAL_INT_TYPE:
                                        {
                                            auto leftOpeCast=std::dynamic_pointer_cast<const SubArithmeticTheorem<NaturalInteger>>(leftOpe);
                                            auto rightOpeCast=std::dynamic_pointer_cast<const ASubArithmeticTheorem<size_t>>(rightOpe);
                                            return std::make_shared<const SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<NaturalInteger> >>>
                                                                    (name,leftOpeCast,rightOpeCast);
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
                                            auto leftOpeCast=std::dynamic_pointer_cast<const ASubArithmeticTheorem<void>>(leftOpe);
                                            auto rightOpeCast=std::dynamic_pointer_cast<const ASubTheorem>(rightOpe);
                                            return std::make_shared<const Theorem<Let<ASubArithmeticTheorem<void>,ASubTheorem>>>
                                                                                            (name,leftOpeCast,rightOpeCast);
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
                                            auto leftOpeCast=std::dynamic_pointer_cast<const ASubTheorem>(leftOpe);
                                            auto rightOpeCast=std::dynamic_pointer_cast<const ASubTheorem>(rightOpe);
                                            return std::make_shared<const Theorem<Let<ASubTheorem,ASubTheorem>>>
                                                                                (name,leftOpeCast,rightOpeCast);
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
                            auto leftOpe=createSubTheorem(name+"+L",leftOpeList,leftOrderedOpeList,varList);
                            auto rightOpe=createSubTheorem(name+"+R",rightOpeList,rightOrderedOpeList,varList);
                            switch(leftOpe->valueType())
                            {
                                case NATURAL_INT_TYPE:
                                {
                                    switch(rightOpe->valueType())
                                    {
                                        case NATURAL_INT_TYPE:
                                        {
                                            auto leftOpeCast=std::dynamic_pointer_cast<const ASubArithmeticTheorem<size_t>>(leftOpe);
                                            auto rightOpeCast=std::dynamic_pointer_cast<const ASubArithmeticTheorem<size_t>>(rightOpe);
                                            return std::make_shared<const SubArithmeticTheorem<Plus<ASubArithmeticTheorem<size_t>,
                                                    ASubArithmeticTheorem<size_t> >>>(name,leftOpeCast,rightOpeCast);
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
                    throw std::runtime_error("Empty SubTheorem");
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
                    return std::make_shared<const SubTheorem<ConstBoolean>>(name,
                                            std::dynamic_pointer_cast<ConstBoolean>(var)->evaluate());
                }
                case VALUE_TYPE::NATURAL_INT_TYPE:
                {
                    return std::make_shared<const ConstSubArithmeticTheorem<ConstNaturalInteger>>(name,
                                                std::dynamic_pointer_cast<ConstNaturalInteger>(var)->evaluate());
                }
                case VALUE_TYPE::NATURAL_INTEGERSET_TYPE:
                {
                    return std::make_shared<const ConstSubArithmeticTheorem<ConstSet<NaturalIntegerSet>>>(name,
                                                std::dynamic_pointer_cast<ConstSet<NaturalIntegerSet>>(var)->evaluate());
                }
                case VALUE_TYPE::BOOLEANSET_TYPE:
                {
                    return std::make_shared<const ConstSubArithmeticTheorem<ConstSet<BooleanSet>>>(name,
                                                std::dynamic_pointer_cast<ConstSet<BooleanSet>>(var)->evaluate());
                }
                default:
                {
                    throw std::runtime_error("No void variable in arithmetic formula");
                }
            }
        }
        else
        {
            switch(var->valueType())
            {
                case VALUE_TYPE::BOOL_TYPE:
                {
                    return std::make_shared<const SubTheorem<Boolean>>(name,
                                            std::dynamic_pointer_cast<Boolean>(var));
                }
                case VALUE_TYPE::NATURAL_INT_TYPE:
                {
                    return std::make_shared<const SubArithmeticTheorem<NaturalInteger>>(name,
                                                std::dynamic_pointer_cast<NaturalInteger>(var));
                }
                case VALUE_TYPE::NATURAL_INTEGERSET_TYPE:
                {
                    return std::make_shared<const SubArithmeticTheorem<VarSet<NaturalIntegerSet>>>(name,
                                                std::dynamic_pointer_cast<VarSet<NaturalIntegerSet>>(var));
                }
                case VALUE_TYPE::BOOLEANSET_TYPE:
                {
                    return std::make_shared<const SubArithmeticTheorem<VarSet<BooleanSet>>>(name,
                                                std::dynamic_pointer_cast<VarSet<BooleanSet>>(var));
                }
                default:
                {
                    throw std::runtime_error("No void variable in arithmetic formula");
                }
            }
        }
    }
    else
    {
        throw std::runtime_error("Empty SubTheorem");
    }
    return nullptr;
}


/**---------------------------------------------------------------
 * Constructor methods from a vector of subProperties
 * ---------------------------------------------------------------
 */
template<>
SubTheorem<And<ASubTheorem>>::SubTheorem(const std::string& name_, const ptr<ASubTheorem>& leftSubProp,
                                         const ptr<ASubTheorem>& rightSubProp):
    ASubPureTheorem(name_,leftSubProp->getExtVars(),rightSubProp->getExtVars(),AND_PROP),
    m_son(std::make_unique<And<ASubTheorem>>(leftSubProp,rightSubProp))
{
    computeAllPaths();
}

template<>
SubTheorem<Equivalent<ASubTheorem>>::SubTheorem(const std::string& name_, const ptr<ASubTheorem>& leftSubProp,
                                                const ptr<ASubTheorem>& rightSubProp):
    ASubPureTheorem(name_,leftSubProp->getExtVars(),rightSubProp->getExtVars(),EQUIV_PROP),
    m_son(std::make_unique<Equivalent<ASubTheorem>>(leftSubProp,rightSubProp))
{
    computeAllPaths();
}

SubTheorem<Hyp<ASubTheorem>>::SubTheorem(const std::string& name_, const std::vector<ptr<ASubTheorem>>& subProps):
    ASubPureTheorem(name_,getDbVarFromTheorems(subProps),HYP_PROP),
    m_son(std::make_unique<Hyp<ASubTheorem>>(subProps))
{
    computeAllPaths();
}

template<>
SubTheorem<Implication<ASubTheorem>>::SubTheorem(const std::string& name_, const ptr<ASubTheorem>& leftSubProp,
                                                 const ptr<ASubTheorem>& rightSubProp):
    ASubPureTheorem(name_,leftSubProp->getExtVars(),rightSubProp->getExtVars(),IMPL_PROP),
    m_son(std::make_unique<Implication<ASubTheorem>>(leftSubProp,rightSubProp))
{
    computeAllPaths();
}

SubTheorem<Not<ASubTheorem>>::SubTheorem(const std::string& name_, const ptr<ASubTheorem>& subProp):
    ASubPureTheorem(name_,subProp->getExtVars(),NOT_PROP),
    m_son(std::make_unique<Not<ASubTheorem>>(subProp))
{
    computeAllPaths();
}

template<>
SubTheorem<Or<ASubTheorem>>::SubTheorem(const std::string& name_, const ptr<ASubTheorem>& leftSubProp, const ptr<ASubTheorem>& rightSubProp):
    ASubPureTheorem(name_,leftSubProp->getExtVars(),rightSubProp->getExtVars(),OR_PROP),
    m_son(std::make_unique<Or<ASubTheorem>>(leftSubProp,rightSubProp))
{
    computeAllPaths();
}

SubTheorem<Boolean>::SubTheorem(const std::string& name_, const std::shared_ptr<Boolean>& son):
    ASubTheorem(name_,DbVar(son),VAR_PROP), m_son(son)
{
    computeAllPaths();
}

SubTheorem<ConstBoolean>::SubTheorem(const std::string& name_, const bool& val):
    ASubTheorem(name_,CONST_PROP), m_son(std::make_unique<ConstBoolean>(val))
{
    computeAllPaths();
}


SubTheorem<ConstBoolean>::SubTheorem(const SubRule<ConstBoolean>& prop):
    ASubTheorem(prop.name(),CONST_PROP), m_son(std::make_unique<ConstBoolean>(prop.evaluate()))
{
    computeAllPaths();
}

/**---------------------------------------------------------------
 * operator[] methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
const ptr<ASubTheorem> &SubTheorem<SubPropertyType>::operator[](const size_t& index) const
{
    return (*m_son)[index];
}

const ptr<ASubTheorem> &SubTheorem<Hyp<ASubTheorem>>::operator[](const size_t& index) const
{
    return (*m_son)[index];
}

const ptr<ASubTheorem> &SubTheorem<Not<ASubTheorem>>::operator[](const size_t&) const
{
    return (*m_son)[0];
}


/**---------------------------------------------------------------
 * arity methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
size_t SubTheorem<SubPropertyType>::arity() const
{
    return m_son->arity();
}

size_t SubTheorem<Hyp<ASubTheorem>>::arity() const
{
    return m_son->arity();
}

size_t SubTheorem<Not<ASubTheorem>>::arity() const
{
    return m_son->arity();
}

size_t SubTheorem<Boolean>::arity() const
{
    return 0;
}

size_t SubTheorem<ConstBoolean>::arity() const
{
    return 0;
}

/**---------------------------------------------------------------
 * evaluate methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
bool SubTheorem<SubPropertyType>::evaluate() const
{
    return m_son->evaluate();
}

bool SubTheorem<Hyp<ASubTheorem>>::evaluate() const
{
    return m_son->evaluate();
}

bool SubTheorem<Not<ASubTheorem>>::evaluate() const
{
    return m_son->evaluate();
}

bool SubTheorem<Boolean>::evaluate() const
{
    return m_son->evaluate();
}

bool SubTheorem<ConstBoolean>::evaluate() const
{
    return m_son->evaluate();
}

/**---------------------------------------------------------------
 * canBeDemonstrated methods
 * ---------------------------------------------------------------
 */
bool SubTheorem<Boolean>::canBeDemonstrated() const
{
    return false;
}

bool SubTheorem<ConstBoolean>::canBeDemonstrated() const
{
    return true;
}

/**---------------------------------------------------------------
 * operator== methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
bool SubTheorem<SubPropertyType>::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

bool SubTheorem<Hyp<ASubTheorem>>::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}


bool SubTheorem<Not<ASubTheorem>>::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

bool SubTheorem<Boolean>::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

bool SubTheorem<ConstBoolean>::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<>
bool SubTheorem<Let<ASubTheorem, ASubTheorem> >::
isEqual(const ASubRule &prop) const
{
    //FIX IT!!!!!!!!! not SubPropertyType
    auto propCast=dynamic_cast<const SubRule<Let<ASubRule, ASubRule> >*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<>
bool SubTheorem<Let<ASubArithmeticTheorem<void>, ASubTheorem> >::
isEqual(const ASubRule &prop) const
{
    //FIX IT!!!!!!!!! not SubPropertyType
    auto propCast=dynamic_cast<const SubRule<Let<ASubArithmeticRule<void>, ASubRule> >*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}


/**---------------------------------------------------------------
 * toString methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
std::string SubTheorem<SubPropertyType>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

std::string SubTheorem<Hyp<ASubTheorem>>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

std::string SubTheorem<Not<ASubTheorem>>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

std::string SubTheorem<Boolean>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

std::string SubTheorem<ConstBoolean>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

/**---------------------------------------------------------------
 * copyTheorem methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
ptr<ASubTheorem> SubTheorem<SubPropertyType>::copyTheorem() const
{
    return std::make_shared<const Theorem<SubPropertyType>>(m_name,(*m_son)[0],(*m_son)[1]);
}

ptr<ASubTheorem> SubTheorem<Hyp<ASubTheorem>>::copyTheorem() const
{
    std::vector<ptr<ASubTheorem>> subProps;
    for(size_t k=0;k<arity();k++)
    {
        subProps.push_back((*m_son)[k]);
    }
    return std::make_shared<const Theorem<Hyp<ASubTheorem>>>(m_name,subProps);
}

ptr<ASubTheorem> SubTheorem<Not<ASubTheorem>>::copyTheorem() const
{
    return std::make_shared<const Theorem<Not<ASubTheorem>>>(m_name,(*m_son)[0]);
}

ptr<ASubTheorem> SubTheorem<Boolean>::copyTheorem() const
{
    return std::make_shared<const Theorem<Boolean>>(m_name,m_son);
}

ptr<ASubTheorem> SubTheorem<ConstBoolean>::copyTheorem() const
{
    return std::make_shared<const Theorem<ConstBoolean>>(m_name,m_son->evaluate());
}

/**---------------------------------------------------------------
 * ruleApply methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
ptr<IISubTheoremFormula> SubTheorem<SubPropertyType>::ruleApply(const IISubRuleFormula &rule,
                                                                std::vector<size_t> &indexes, const size_t &actionKey) const
{
    Arity index=indexes[0];
    indexes.erase(indexes.begin());
    auto ruleCast=dynamic_cast<const ASubRule*>(&rule);
    if(index==0)
    {
        return std::make_shared<const SubTheorem<SubPropertyType>>(name(),
        std::dynamic_pointer_cast<const ASubTheorem>(ruleCast->applyAnnexe(actionKey,(*m_son)[0],indexes)), (*m_son)[1]);
    }
    else
    {
        return std::make_shared<const SubTheorem<SubPropertyType>>(name(),
        (*m_son)[0], std::dynamic_pointer_cast<const ASubTheorem>(ruleCast->applyAnnexe(actionKey,(*m_son)[1],indexes)));
    }
}

ptr<IISubTheoremFormula> SubTheorem<Hyp<ASubTheorem>>::ruleApply(const IISubRuleFormula &rule,
                                                                 std::vector<size_t> &indexes, const size_t &actionKey) const
{
    Arity index=indexes[0];
    indexes.erase(indexes.begin());
    std::vector<ptr<ASubTheorem>> sons;
    for(size_t k=0;k<index;k++)
    {
        sons.push_back((*m_son)[k]);
    }
    auto ruleCast=dynamic_cast<const ASubRule*>(&rule);
    sons.push_back(std::dynamic_pointer_cast<const ASubTheorem>(ruleCast->applyAnnexe(actionKey,((*m_son)[index]),indexes)));

    for(size_t k=index+1;k<m_son->arity();k++)
    {
        sons.push_back((*m_son)[k]);
    }

    return std::make_shared<const SubTheorem<SubPropertyType>>(name(),sons);
}

ptr<IISubTheoremFormula> SubTheorem<Not<ASubTheorem>>::ruleApply(const IISubRuleFormula &rule,
                                                                 std::vector<size_t> &indexes, const size_t &actionKey) const
{
    indexes.erase(indexes.begin());
    auto ruleCast=dynamic_cast<const ASubRule*>(&rule);
    return std::make_shared<const SubTheorem<Not<ASubTheorem>>>(name(),
    std::dynamic_pointer_cast<const ASubTheorem>(ruleCast->applyAnnexe(actionKey,(*m_son)[0],indexes)));
}

ptr<IISubTheoremFormula> SubTheorem<Boolean>::ruleApply(const IISubRuleFormula &, std::vector<size_t> &, const size_t &) const
{
    throw std::runtime_error("SubTheorem Boolean cannot call ruleApply method");
}

ptr<IISubTheoremFormula> SubTheorem<ConstBoolean>::ruleApply(const IISubRuleFormula &, std::vector<size_t> &, const size_t &) const
{
    throw std::runtime_error("SubTheorem ConstBoolean cannot call ruleApply method");
}

template<SubTheoremProperty SubPropertyType>
bool SubTheorem<SubPropertyType>::operator==(const SubTheorem &prop) const
{
    return *m_son==(prop->getSon());
}

/**---------------------------------------------------------------
 * computeAllPaths methods
 * ---------------------------------------------------------------
 */
const std::vector<std::vector<Arity> >& SubTheorem<Hyp<ASubTheorem>>::computeAllPaths()
{
    if(!m_allPaths.size())
    {
        m_allPaths.push_back({});
        for(size_t i=0;i<m_son->arity();i++)
        {
            std::vector<std::vector<Arity>> sonPath=(*m_son)[i]->getAllPaths();
            if(sonPath.size())
            {
                for(size_t k=0; k<sonPath.size();k++)
                {
                    std::vector<Arity> crtPath=sonPath[k];
                    crtPath.insert(crtPath.begin(),i);
                    m_allPaths.push_back(crtPath);
                }
            }
            else
            {
                m_allPaths.push_back({i});
            }
        }
    }
    return m_allPaths;
}

const std::vector<std::vector<Arity> >& SubTheorem<Not<ASubTheorem>>::computeAllPaths()
{
    if(!m_allPaths.size())
    {
        m_allPaths.push_back({});
        std::vector<std::vector<Arity>> sonPath=(*m_son)[0]->getAllPaths();
        for(size_t k=0; k<sonPath.size();k++)
        {
            std::vector<Arity> crtPath=sonPath[k];
            crtPath.insert(crtPath.begin(),0);
            m_allPaths.push_back(crtPath);
        }
    }
    return m_allPaths;
}


const std::vector<std::vector<Arity> >& SubTheorem<Boolean>::computeAllPaths()
{
    if(!m_allPaths.size())
    {
        m_allPaths.push_back({});
    }
    return m_allPaths;
}

const std::vector<std::vector<Arity> >& SubTheorem<ConstBoolean>::computeAllPaths()
{
    if(!m_allPaths.size())
    {
        m_allPaths.push_back({});
    }
    return m_allPaths;
}


/**---------------------------------------------------------------
 * getSon methods
 * ---------------------------------------------------------------
 */
const Hyp<ASubTheorem> &SubTheorem<Hyp<ASubTheorem>>::getSon() const
{
    return *(m_son.get());
}

const Not<ASubTheorem> &SubTheorem<Not<ASubTheorem>>::getSon() const
{
    return *(m_son.get());
}
const Boolean &SubTheorem<Boolean>::getSon() const
{
    return *(m_son.get());
}

const ConstBoolean &SubTheorem<ConstBoolean>::getSon() const
{
    return *(m_son.get());
}
