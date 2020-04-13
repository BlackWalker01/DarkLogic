/*===--- operator.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines operator which is base class for all operators in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_OPERATOR_H
#define DARK_LOGIC_OPERATOR_H

#include <vector>
#include <climits>
#include <string>
#include <tuple>
#include "ioperator.h"


namespace N_DarkLogic {

class IProposition;

template<FunOpe FunType_>
class Operator: public IOperator
{
public:

    using FunType=FunType_;
    using ValueType = typename FunType::ValueType;
    using SubFormulaType = typename FunType::SubFormulaType;
    using SubRuleFormulaType = ToRule<SubFormulaType>;
    using SubTheoremFormulaType = ToTheorem<SubFormulaType>;

    Operator();
    template<typename HeadType, typename ...QueueType>
    Operator(Name name, HeadType& propHead, QueueType& ...queueHead);


    //parser methods
    Name name() const override final;
    Arity arity() const override final;
    unsigned short priority() const override final;
    Associativity associativity() const override final;
    std::string symbol() const  override final;
    NbArgBefore nbArgBefore() const;

    ValueType evaluate() const;

    std::string toString(unsigned short priorityParent=1000) const;

    ~Operator() = default;

    //Attributes
    const FunType m_fun;
};

#include "operator.hxx"
}
#endif // DARK_LOGIC_OPERATOR_H
