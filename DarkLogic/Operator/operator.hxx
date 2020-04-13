/*===--- operator.hxx - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements operator class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/


#ifndef DARK_LOGIC_OPERATOR_HXX
#define DARK_LOGIC_OPERATOR_HXX

template<FunOpe FunType>
Operator<FunType>::Operator(): m_fun()
{

}

template<FunOpe FunType> template<typename HeadType, typename ...QueueType>
Operator<FunType>::Operator(Name /*name*/, HeadType& propHead, QueueType& ...queueHead):
    IOperator(), m_fun(propHead,queueHead...)
{

}


/**
 * parser methods
 */

template<FunOpe FunType>
Name Operator<FunType>::name() const
{
    return FunType::name();
}

template<FunOpe FunType>
Arity Operator<FunType>::arity() const
{
    return m_fun.arity();
}

template<FunOpe FunType>
unsigned short Operator<FunType>::priority() const
{
    return FunType::priority();
}

template<FunOpe FunType>
Associativity Operator<FunType>::associativity() const
{
    return FunType::associativity();
}

template<FunOpe FunType>
std::string Operator<FunType>::symbol() const
{
    return FunType::symbol();
}

template<FunOpe FunType>
NbArgBefore Operator<FunType>::nbArgBefore() const
{
    return FunType::nbArgBefore();
}




template<FunOpe FunType>
typename Operator<FunType>::ValueType Operator<FunType>::evaluate() const
{
    return m_fun();
}

template<FunOpe FunType>
std::string Operator<FunType>::toString(unsigned short priorityParent) const
{
    return m_fun.toString(priorityParent);
}

#endif // DARK_LOGIC_OPERATOR_HXX
