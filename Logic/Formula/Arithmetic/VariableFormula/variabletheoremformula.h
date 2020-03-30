#ifndef VARIABLETHEOREMFORMULA_H
#define VARIABLETHEOREMFORMULA_H
#include "Formula/Arithmetic/subarithmetictheorem.h"

namespace N_Logic
{
template<typename VarType>
class SubArithmeticTheorem:
        public ASubArithmeticTheorem<typename VarType::ValueType>
{
public:
    typedef VarType SubOperatorType;
    typedef typename SubOperatorType::ValueType ValueType;

    SubArithmeticTheorem(const std::string& name_, const std::shared_ptr<VarType>& var);

    size_t arity() const override final;
    ValueType evaluate() const override final;

    const SubOperatorType& getSon() const;
    std::string toString(unsigned short priorityParent=1000) const override final;
    const std::vector<std::vector<Arity>>& computeAllPaths() override final;

    void operator=(const ValueType& val) const;

    bool isEqual(const ASubArithmeticRule<ValueType>& prop) const override final;
    bool isEqual(const ASubArithmeticTheorem<ValueType>& prop) const override final;
    bool operator==(const SubArithmeticTheorem& prop) const;
    bool operator==(const SubArithmeticRule<SubOperatorType>& prop) const;

    ~SubArithmeticTheorem() override = default;

    const std::shared_ptr<SubOperatorType> m_son;
};

template<typename VarType>
SubArithmeticTheorem<VarType>::SubArithmeticTheorem(const std::string &name_, const std::shared_ptr<VarType> &var):
    ASubArithmeticTheorem<typename VarType::ValueType> (name_,ToArithType<ValueType>::convert()), m_son(var)
{
    static_assert (!VarType::isConstantExpr(),"VarType must be Variable type in ConstSubArithmeticRule");
}

template<typename VarType>
size_t SubArithmeticTheorem<VarType>::arity() const
{
    return 0;
}

template<typename VarType>
typename SubArithmeticTheorem<VarType>::ValueType SubArithmeticTheorem<VarType>::evaluate() const
{
    return m_son->evaluate();
}

template<typename VarType>
const typename SubArithmeticTheorem<VarType>::SubOperatorType &SubArithmeticTheorem<VarType>::getSon() const
{
    return *(m_son.get());
}

template<typename VarType>
std::string SubArithmeticTheorem<VarType>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<typename VarType>
const std::vector<std::vector<Arity> > &SubArithmeticTheorem<VarType>::computeAllPaths()
{
    if(!this->m_allPaths.size())
    {
        this->m_allPaths.push_back({});
    }
    return this->m_allPaths;
}

template<typename VarType>
void SubArithmeticTheorem<VarType>::operator=(const ValueType &val) const
{
    *m_son=val;
}

template<typename VarType>
bool SubArithmeticTheorem<VarType>::isEqual(const ASubArithmeticRule<SubArithmeticTheorem::ValueType> &prop) const
{
    auto propCast=
            dynamic_cast<const SubArithmeticRule<VarType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<typename VarType>
bool SubArithmeticTheorem<VarType>::isEqual(const ASubArithmeticTheorem<SubArithmeticTheorem::ValueType> &prop) const
{
    auto propCast=
            dynamic_cast<const SubArithmeticTheorem<VarType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<typename VarType>
bool SubArithmeticTheorem<VarType>::operator==(const SubArithmeticTheorem &prop) const
{
    return *m_son==(prop.getSon());
}

template<typename VarType>
bool SubArithmeticTheorem<VarType>::operator==(const SubArithmeticRule<SubArithmeticTheorem::SubOperatorType> &prop) const
{
    return *m_son==(prop.getSon());
}
}

#endif // VARIABLETHEOREMFORMULA_H
