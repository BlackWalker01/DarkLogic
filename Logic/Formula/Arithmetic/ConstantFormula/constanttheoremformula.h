#ifndef CONSTANTTHEOREMFORMULA_H
#define CONSTANTTHEOREMFORMULA_H
#include "Formula/Arithmetic/subarithmetictheorem.h"

namespace N_Logic
{
template<ArithConstantType VarType>
class ConstSubArithmeticTheorem:
        public ASubArithmeticTheorem<typename VarType::ValueType>
{
public:
    typedef VarType SubOperatorType;
    typedef typename SubOperatorType::ValueType ValueType;

    ConstSubArithmeticTheorem(const ValueType& var);

    size_t arity() const override final;
    ValueType evaluate() const override final;
    constexpr ArithType type() const override final;

    const SubOperatorType& getSon() const;
    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;
    const std::vector<std::vector<Arity>>& computeAllPaths() override final;

    bool isEqual(const ASubArithmeticRule<ValueType>& prop) const override final;
    bool isEqual(const ASubArithmeticTheorem<ValueType>& prop) const override final;
    bool operator==(const ConstSubArithmeticTheorem& prop) const;
    bool operator==(const SubArithmeticRule<SubOperatorType>& prop) const;

    ~ConstSubArithmeticTheorem() override = default;
private:
    const std::unique_ptr<const SubOperatorType> m_son;
};

template<ArithmeticType ValueType> 
struct ToRuleStruct<ConstSubArithmeticTheorem<ValueType>> { using Type = ConstSubArithmeticRule<ValueType>; };

template<ArithConstantType VarType>
ConstSubArithmeticTheorem<VarType>::ConstSubArithmeticTheorem(const ValueType &var):
    m_son(std::make_unique<SubOperatorType>(var))
{
    static_assert (VarType::isConstantExpr(),"VarType must be Constant type in ConstSubArithmeticRule");
    computeAllPaths();
}

template<ArithConstantType VarType>
size_t ConstSubArithmeticTheorem<VarType>::arity() const
{
    return 0;
}

template<ArithConstantType VarType>
typename ConstSubArithmeticTheorem<VarType>::ValueType ConstSubArithmeticTheorem<VarType>::evaluate() const
{
    return m_son->evaluate();
}

template<ArithConstantType VarType>
inline constexpr ArithType ConstSubArithmeticTheorem<VarType>::type() const
{
    return ToArithType<ValueType>::convert();
}


template<ArithConstantType VarType>
const typename ConstSubArithmeticTheorem<VarType>::SubOperatorType &ConstSubArithmeticTheorem<VarType>::getSon() const
{
    return *(m_son.get());
}

template<ArithConstantType VarType>
std::string ConstSubArithmeticTheorem<VarType>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<ArithConstantType VarType>
inline const DbVar* ConstSubArithmeticTheorem<VarType>::getExtVars() const
{
    return nullptr;
}

template<ArithConstantType VarType>
const std::vector<std::vector<Arity> > &ConstSubArithmeticTheorem<VarType>::computeAllPaths()
{
    if(!this->m_allPaths.size())
    {
        this->m_allPaths.push_back({});
    }
    return this->m_allPaths;
}

template<ArithConstantType VarType>
bool ConstSubArithmeticTheorem<VarType>::isEqual
(const ASubArithmeticRule<ConstSubArithmeticTheorem::ValueType> &prop) const
{
    auto propCast=
            dynamic_cast<const SubArithmeticRule<VarType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==propCast->getSon();
    }
    else
    {
        return false;
    }
}

template<ArithConstantType VarType>
bool ConstSubArithmeticTheorem<VarType>::isEqual
(const ASubArithmeticTheorem<ConstSubArithmeticTheorem::ValueType> &prop) const
{
    auto propCast=
            dynamic_cast<const ConstSubArithmeticTheorem<VarType>*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<ArithConstantType VarType>
bool ConstSubArithmeticTheorem<VarType>::operator==
(const ConstSubArithmeticTheorem &prop) const
{
    return *m_son==(prop.getSon());
}

template<ArithConstantType VarType>
bool ConstSubArithmeticTheorem<VarType>::operator==
(const SubArithmeticRule<ConstSubArithmeticTheorem::SubOperatorType> &prop) const
{
    return *m_son==(prop.getSon());
}
}

#endif // CONSTANTTHEOREMFORMULA_H
