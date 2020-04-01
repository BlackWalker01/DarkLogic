#ifndef OPERATOR_HXX
#define OPERATOR_HXX

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

#endif // OPERATOR_HXX
