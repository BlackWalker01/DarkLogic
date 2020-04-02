#ifndef OPERATOR_H
#define OPERATOR_H

#include <vector>
#include <climits>
#include <string>
#include <tuple>
#include "ioperator.h"


namespace N_Logic {

class IProposition;

template<FunOpe FunType_>
class Operator: public IOperator
{
public:

    using FunType=FunType_;
    typedef typename FunType::ValueType ValueType;

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
#endif // OPERATOR_H
