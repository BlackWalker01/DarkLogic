/*===---  variable.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file includes all boolean term includes in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef BLACK_DARK_LOGIC_VARIABLE_H
#define BLACK_DARK_LOGIC_VARIABLE_H
#include "avariable.h"
#include "variableexception.h"

namespace N_DarkLogic {
template<SetDerived SetType_>
class Variable: public AVariable
{
public:
    using SetType = SetType_;
    using ValueType = typename SetType::Type;

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
    void operator=(const ValueType& val) const;
    void unset() const;

    //comparison operators
    template<SetDerived SetType2>
    constexpr bool operator==(const Variable<SetType2>&) const;
    constexpr bool operator==(const Variable<SetType>& var) const;
    bool operator==(const ValueType& val) const;

    virtual ~Variable() = default;

protected:
    const std::unique_ptr<std::unique_ptr<ValueType>> m_val;
};

template<SetDerived SetType_>
Variable<SetType_>::Variable(const std::string& name_): AVariable(name_), 
m_val(std::make_unique<std::unique_ptr<ValueType>>(nullptr))
{

}


template<SetDerived SetType> template<SetDerived SetType2>
constexpr bool Variable<SetType>::operator==(const Variable<SetType2>&) const
{
    return false;
}

template<SetDerived SetType>
constexpr bool Variable<SetType>::operator==(const Variable<SetType>& var) const
{
    return this->m_id==var.m_id;
}

template<SetDerived SetType>
void Variable<SetType>::operator=(const ValueType& val) const
{
    (*m_val)=std::make_unique(val);
}

template<SetDerived SetType>
inline void Variable<SetType>::unset() const
{
    (*m_val) = nullptr;
}

template<SetDerived SetType>
bool Variable<SetType>::operator==(const ValueType& val) const
{
    if(*m_val)
    {
        return **m_val==val;
    }
    throw VariableException(this->m_name);
}

template<SetDerived SetType>
constexpr typename Variable<SetType>::ValueType Variable<SetType>::evaluate() const
{
    if(*m_val)
    {
        return **m_val;
    }
    throw VariableException(this->m_name);
}

template<SetDerived SetType>
bool Variable<SetType>::isConstant() const
{
    return false;
}

#include "variable.hxx"
}
#endif // DARK_LOGIC_VARIABLE_H
