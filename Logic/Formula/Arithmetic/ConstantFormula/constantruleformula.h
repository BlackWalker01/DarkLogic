#ifndef CONSTANTRULEFORMULA_H
#define CONSTANTRULEFORMULA_H
#include "Formula/Arithmetic/subarithmeticrule.h"
#include "Formula/dbvarprop.h"

namespace N_Logic
{

template<typename VarType>
class ConstSubArithmeticTheorem;

template<typename VarType>
class ConstSubArithmeticRule:
        public ASubArithmeticRule<typename VarType::ValueType>
{
public:
    typedef VarType SubOperatorType;
    typedef typename SubOperatorType::ValueType ValueType;
    typedef typename
    ASubArithmeticRule<typename VarType::ValueType>::ATheoremType ATheoremType;

    ConstSubArithmeticRule(const std::string& name_, const ValueType& var);

    size_t arity() const override final;
    ValueType evaluate() const override final;

    //internal methods
    bool identifyPriv(const ptr<ATheoremType>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;

    const VarType& getSon() const;

    bool operator==(const ASubArithmeticRule<ValueType>& prop) const override final;
    bool operator==(const ASubArithmeticTheorem<ValueType>& prop) const override final;
    bool operator==(const ConstSubArithmeticRule& prop) const;
    bool operator==(const ConstSubArithmeticTheorem<SubOperatorType>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;

    ~ConstSubArithmeticRule() override = default;

protected:
    const std::unique_ptr<const SubOperatorType> m_son;
};

template<typename VarType>
ConstSubArithmeticRule<VarType>::ConstSubArithmeticRule(const std::string &name_, const ValueType &var):
    ASubArithmeticRule<typename VarType::ValueType>(name_,ToArithType<ValueType>::convert()),
    m_son(std::make_unique<SubOperatorType>(var))
{
    static_assert (VarType::isConstantExpr(),"VarType must be Constant type in ConstSubArithmeticRule");
    static_assert(!std::is_same_v<VarType,ConstBoolean >,"VarType cannot be Boolean, use SubRule<Boolean> instead");
}

template<typename VarType>
size_t ConstSubArithmeticRule<VarType>::arity() const
{
    return 0;
}

template<typename VarType>
typename ConstSubArithmeticRule<VarType>::ValueType ConstSubArithmeticRule<VarType>::evaluate() const
{
    return m_son->evaluate();
}


template<typename VarType>
bool ConstSubArithmeticRule<VarType>::
identifyPriv(const ptr<typename ConstSubArithmeticRule<VarType>::ATheoremType> &prop, DbVarProp &/*dbVarProp*/) const
{
   return (*this)==*prop;
}

template<typename VarType>
ptr<typename ConstSubArithmeticRule<VarType>::ATheoremType>
ConstSubArithmeticRule<VarType>::
applyPriv(const std::string &thName, DbVarProp &/*dbVarProp*/) const
{
    return std::make_shared<const ConstSubArithmeticTheorem<VarType>>(thName,m_son->evaluate());
}


template<typename VarType>
ptr<typename ConstSubArithmeticRule<VarType>::ATheoremType>
ConstSubArithmeticRule<VarType>::applyFirstPriv(const std::string &thName, DbVarProp &/*dbVarProp*/) const
{
    return std::make_shared<const ConstSubArithmeticTheorem<VarType>>(thName,m_son->evaluate());
}


template<typename VarType>
const VarType &ConstSubArithmeticRule<VarType>::getSon() const
{
    return *(m_son.get());
}

template<typename VarType>
bool ConstSubArithmeticRule< VarType>::
operator==(const ASubArithmeticTheorem<ValueType>& prop) const
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

template<typename VarType>
bool ConstSubArithmeticRule< VarType>::
operator==(const ASubArithmeticRule<ValueType>& prop) const
{
    auto propCast=dynamic_cast<const ConstSubArithmeticRule*>(&prop);
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
bool ConstSubArithmeticRule<VarType>::
operator==(const ConstSubArithmeticRule<VarType> &prop) const
{
    return *m_son==(prop.getSon());
}

template<typename VarType>
bool ConstSubArithmeticRule<VarType>::operator==(const ConstSubArithmeticTheorem<VarType> &prop) const
{
    return *m_son==(prop.getSon());
}

template<typename VarType>
std::string ConstSubArithmeticRule<VarType>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}
}

#endif // CONSTANTRULEFORMULA_H
