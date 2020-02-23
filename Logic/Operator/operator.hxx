#ifndef OPERATOR_HXX
#define OPERATOR_HXX

template<typename FunType>
Operator<FunType>::Operator(): m_fun()
{

}

template<typename FunType> template<typename HeadType, typename ...QueueType>
Operator<FunType>::Operator(Name /*name*/, HeadType& propHead, QueueType& ...queueHead):
    IOperator(), m_fun(propHead,queueHead...)
{

}


/**
 * parser methods
 */

template<typename FunType>
Name Operator<FunType>::name() const
{
    return FunType::name();
}

template<typename FunType>
Arity Operator<FunType>::arity() const
{
    return m_fun.arity();
}

template<typename FunType>
unsigned short Operator<FunType>::priority() const
{
    return FunType::priority();
}

template<typename FunType>
Associativity Operator<FunType>::associativity() const
{
    return FunType::associativity();
}

template<typename FunType>
std::string Operator<FunType>::symbol() const
{
    return FunType::symbol();
}

template<typename FunType>
NbArgBefore Operator<FunType>::nbArgBefore() const
{
    return FunType::nbArgBefore();
}




template<typename FunType>
typename Operator<FunType>::ValueType Operator<FunType>::evaluate() const
{
    return m_fun();
}

template<typename FunType>
std::string Operator<FunType>::toString(unsigned short priorityParent) const
{
    return m_fun.toString(priorityParent);
}

#endif // OPERATOR_HXX
