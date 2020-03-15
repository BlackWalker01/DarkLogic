#ifndef VARIABLE_H
#define VARIABLE_H
#include "term.h"

namespace N_Logic {
template<typename SetType>
class Variable: public AbstractTerm
{
public:
    typedef typename SetType::Type ValueType;

    Variable(const std::string& name_);
    Variable(const Variable& var) = delete;

    //get operator
    constexpr ValueType evaluate() const;
    bool isConstant() const override;
    static constexpr bool isConstantExpr()
    {
        return false;
    }

    //set operator
    void operator=(const ValueType& val);

    //comparison operators
    template<typename SetType2>
    constexpr bool operator==(const Variable<SetType2>&) const;
    constexpr bool operator==(const Variable<SetType>& var) const;
    bool operator==(const ValueType& val) const;

    virtual ~Variable() = default;

protected:
    std::unique_ptr<ValueType> m_val;
};

template<typename SetType_>
Variable<SetType_>::Variable(const std::string& name_): AbstractTerm(name_), m_val(nullptr)
{

}


template<typename SetType> template<typename SetType2>
constexpr bool Variable<SetType>::operator==(const Variable<SetType2>&) const
{
    return false;
}

template<typename SetType>
constexpr bool Variable<SetType>::operator==(const Variable<SetType>& var) const
{
    return this->m_name==var.m_name;
}

template<typename SetType>
void Variable<SetType>::operator=(const ValueType& val)
{
    m_val=std::make_unique(val);
}

template<typename SetType>
bool Variable<SetType>::operator==(const ValueType& val) const
{
    if(m_val)
    {
        return *m_val==val;
    }
    throw std::runtime_error("Variable "+this->m_name+" not initialized");
}

template<typename SetType>
constexpr typename Variable<SetType>::ValueType Variable<SetType>::evaluate() const
{
    if(m_val)
    {
        return *m_val;
    }
    throw std::runtime_error("Variable "+this->m_name+" not initialized");
}

template<typename SetType>
bool Variable<SetType>::isConstant() const
{
    return false;
}

#include "variable.hxx"
}
#endif // VARIABLE_H
