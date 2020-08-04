/*===--- formula.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements formula class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/


#include "formula.h"
#include "formula.h"
#include "Operator/ioperator.h"
#include "Variable/term.h"
#include "Set/booleanset.h"
#include "Set/naturalintegerset.h"
#include "Variable/allvariable.h"
#include "Variable/dbvar.h"
#include "Formula/Boolean/asubrule.h"
#include "Formula/Boolean/asubtheorem.h"

using namespace N_DarkLogic;

template<>
VALUE_TYPE AbstractFormula<void>::valueType() const
{
    return VOID_TYPE;
}

std::string N_DarkLogic::FormulaHelper::getVarNameFromId(const IDVar& idVar)
{
    return AVariable::getVarNameFromId(idVar);
}

template<>
VALUE_TYPE AbstractFormula<bool>::valueType() const
{
    return BOOL_TYPE;
}

template<>
VALUE_TYPE AbstractFormula<UInt>::valueType() const
{
    return NATURAL_INT_TYPE;
}

template<>
VALUE_TYPE AbstractFormula<BooleanSet>::valueType() const
{
    return BOOLEANSET_TYPE;
}

template<>
VALUE_TYPE AbstractFormula<NaturalIntegerSet>::valueType() const
{
    return NATURAL_INTEGERSET_TYPE;
}

std::vector<N_DarkLogic::OperatorOrdering> N_DarkLogic::orderOperator(std::vector<OperatorOrdering> &opeList)
{
    if(opeList.size()<=1)
    {
        return opeList;
    }
    else
    {
        auto mid=opeList.begin()+opeList.size()/2;
        std::vector<OperatorOrdering> opeList1(opeList.begin(),mid);
        std::vector<OperatorOrdering> opeList2(mid,opeList.end());
        return fusion(orderOperator(opeList1),orderOperator(opeList2));
    }
}

/**
 * return 1 if ope1 > ope2 (i.e closer to variables)
 * return -1 if ope1 < ope2
 * else return 0
*/
int compareTab(const std::vector<HypParams>& tab1, const std::vector<HypParams>& tab2)
{
    size_t minSize = tab1.size() < tab2.size() ? tab1.size() : tab2.size();
    for (size_t k = 0; k < minSize; k++)
    {
        if (tab2[k].idx > tab1[k].idx)
        {
            return -1;
        }
        else if (tab2[k].idx == tab1[k].idx)
        {
            if (tab2[k].argIdx > tab1[k].argIdx)
            {
                return -1;
            }
            else if (tab2[k].argIdx < tab1[k].argIdx)
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief N_LogicGame::fusion
 * @param opeList1 = ope1::q1
 * @param opeList2 = ope2::q2
 * @return
 */
std::vector<N_DarkLogic::OperatorOrdering> N_DarkLogic::fusion(const std::vector<OperatorOrdering> &opeList1,
                                                        const std::vector<OperatorOrdering> &opeList2)
{
    if(opeList1.size()==0)
    {
        return opeList2;
    }
    else if(opeList2.size()==0)
    {
        return opeList1;
    }
    else
    {
        // check function calls
        auto cmp = compareTab(opeList1[0].hyps, opeList2[0].hyps);
        if (cmp < 0)
        {
            std::vector<OperatorOrdering> t1(opeList1.begin(), opeList1.begin() + 1);
            std::vector<OperatorOrdering> queueOpeList1(opeList1.begin() + 1, opeList1.end());
            std::vector<OperatorOrdering> t2 = fusion(queueOpeList1, opeList2);
            auto ret = t1;
            ret.insert(ret.end(), t2.begin(), t2.end());
            return ret;
        }
        else if (cmp > 0)
        {
            std::vector<OperatorOrdering> t1(opeList2.begin(), opeList2.begin() + 1);
            std::vector<OperatorOrdering> queueOpeList2(opeList2.begin() + 1, opeList2.end());
            std::vector<OperatorOrdering> t2 = fusion(opeList1, queueOpeList2);
            auto ret = t1;
            ret.insert(ret.end(), t2.begin(), t2.end());
            return ret;
        }
        // if ope1 and ope2 are in the same function and in the same argument
        else
        {
            //if ope1 is in less parenthesis than ope2, then return ope1::fusion(q1,opeList2)
            if (opeList1[0].nbPar < opeList2[0].nbPar)
            {
                std::vector<OperatorOrdering> t1(opeList1.begin(), opeList1.begin() + 1);
                std::vector<OperatorOrdering> queueOpeList1(opeList1.begin() + 1, opeList1.end());
                std::vector<OperatorOrdering> t2 = fusion(queueOpeList1, opeList2);
                auto ret = t1;
                ret.insert(ret.end(), t2.begin(), t2.end());
                return ret;
            }
            //if ope2 is in less parenthesis than ope1, then return ope2::fusion(opeList1,q2)
            else if (opeList1[0].nbPar > opeList2[0].nbPar)
            {
                std::vector<OperatorOrdering> t1(opeList2.begin(), opeList2.begin() + 1);
                std::vector<OperatorOrdering> queueOpeList2(opeList2.begin() + 1, opeList2.end());
                std::vector<OperatorOrdering> t2 = fusion(opeList1, queueOpeList2);
                auto ret = t1;
                ret.insert(ret.end(), t2.begin(), t2.end());
                return ret;
            }
            //if ope1 and ope2 are in the same parenthesis
            else
            {
                if (opeList1[0].ope->priority() > opeList2[0].ope->priority())
                {
                    std::vector<OperatorOrdering> t1(opeList1.begin(), opeList1.begin() + 1);
                    std::vector<OperatorOrdering> queueOpeList1(opeList1.begin() + 1, opeList1.end());
                    std::vector<OperatorOrdering> t2 = fusion(queueOpeList1, opeList2);
                    auto ret = t1;
                    ret.insert(ret.end(), t2.begin(), t2.end());
                    return ret;
                }
                else if (opeList1[0].ope->priority() < opeList2[0].ope->priority())
                {
                    std::vector<OperatorOrdering> t1(opeList2.begin(), opeList2.begin() + 1);
                    std::vector<OperatorOrdering> queueOpeList2(opeList2.begin() + 1, opeList2.end());
                    std::vector<OperatorOrdering> t2 = fusion(opeList1, queueOpeList2);
                    auto ret = t1;
                    ret.insert(ret.end(), t2.begin(), t2.end());
                    return ret;
                }
                else
                {
                    switch (opeList1[0].ope->associativity())
                    {
                    case Associativity::LEFT:
                    {
                        std::vector<OperatorOrdering> t1(opeList1.begin(), opeList1.begin() + 1);
                        std::vector<OperatorOrdering> queueOpeList1(opeList1.begin() + 1, opeList1.end());
                        std::vector<OperatorOrdering> t2 = fusion(queueOpeList1, opeList2);
                        auto ret = t1;
                        ret.insert(ret.end(), t2.begin(), t2.end());
                        return ret;
                    }
                    case Associativity::RIGHT:
                    {
                        std::vector<OperatorOrdering> t1(opeList2.begin(), opeList2.begin() + 1);
                        std::vector<OperatorOrdering> queueOpeList2(opeList2.begin() + 1, opeList2.end());
                        std::vector<OperatorOrdering> t2 = fusion(opeList1, queueOpeList2);
                        auto ret = t1;
                        ret.insert(ret.end(), t2.begin(), t2.end());
                        return ret;
                    }
                    }
                }
            }
        }
    }
    std::vector<OperatorOrdering> ret;
    return ret;
}



N_DarkLogic::OperatorOrdering::OperatorOrdering(): ope(nullptr), nbPar(0), nbArgs(0), foundCcl(false)
{

}

/*N_DarkLogic::OperatorOrdering::OperatorOrdering(const ptr<IOperator>& ope_, const size_t& nbPar_):
    ope(ope_), nbPar(nbPar_), argIndex(0), nbArgs(0)
{
}*/

N_DarkLogic::OperatorOrdering::OperatorOrdering(const ptr<IOperator> &ope_, const size_t &nbPar_, const std::vector<HypParams>& hypStack):
    ope(ope_), nbPar(nbPar_), nbArgs(0), hyps(hypStack), foundCcl(false)
{

}

N_DarkLogic::OperatorOrdering::OperatorOrdering(const OperatorOrdering &opeOrdering): ope(opeOrdering.ope),
    nbPar(opeOrdering.nbPar), nbArgs(opeOrdering.nbArgs), hyps(opeOrdering.hyps),
    foundCcl(opeOrdering.foundCcl)
{

}

size_t N_DarkLogic::OperatorOrdering::argIndex()
{
    return hyps.back().argIdx;
}

N_DarkLogic::VariableContainer::VariableContainer(const std::shared_ptr<AbstractTerm> &term): type(VALUE_TYPE::VOID_TYPE), var(term)
{

}

void VariableContainer::init()
{
    std::string name=var->name();
    switch (type)
    {
        case VOID_TYPE:
        {
            throw std::runtime_error("Cannot initialize variable with no type");
        }
        case BOOL_TYPE:
        {
            if (name.substr(0, 3) == "HYP")
            {
                var = std::make_shared<HypBoolean>(name);
            }
            else
            {
                var = std::make_shared<Boolean>(name);
            }
            
            break;
        }
        case NATURAL_INT_TYPE:
        {
            var=std::make_shared<NaturalInteger>(name);
            break;
        }
        case BOOLEANSET_TYPE:
        {
            var=std::make_shared<VarSet<BooleanSet>>(name);
            break;
        }
        case NATURAL_INTEGERSET_TYPE:
        {
            var=std::make_shared<VarSet<NaturalIntegerSet>> (name);
            break;
        }
    }
}


/**
 * @brief DbVarContainer
 */

void DbVarContainer::insert(const std::shared_ptr<VariableContainer> &var)
{
    db[var->var->name()]=var;
}

std::shared_ptr<VariableContainer> &DbVarContainer::find(const std::string &name)
{
    return db[name];
}

bool DbVarContainer::contains(const std::string &name) const
{
    return db.find(name)!=db.end();
}


std::vector<const DbVar*> N_DarkLogic::getDbVarFromRules(const std::vector<ptr<ASubRule> > &formulas)
{
    std::vector<const DbVar*> ret;
    for(size_t k=0;k<formulas.size();k++)
    {
        ret.push_back(formulas[k]->getExtVars());
    }
    return ret;
}

std::vector<const DbVar*> N_DarkLogic::getDbVarFromTheorems(const std::vector<ptr<ASubTheorem> > &formulas)
{
    std::vector<const DbVar*> ret;
    for(size_t k=0;k<formulas.size();k++)
    {
        ret.push_back(formulas[k]->getExtVars());
    }
    return ret;
}

HypParams::HypParams(const size_t& idx_, const size_t& argIdx_):
    idx(idx_), argIdx(argIdx_)
{

}
