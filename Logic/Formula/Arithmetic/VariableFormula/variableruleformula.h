#ifndef VARIABLERULEFORMULA_H
#define VARIABLERULEFORMULA_H
#include "Formula/Arithmetic/subarithmeticrule.h"
#include "Formula/dbvarprop.h"

namespace N_Logic
{
template<typename VarType>
class SubArithmeticRule:
        public ASubArithmeticRule<typename VarType::ValueType>
{
public:
    typedef VarType SubOperatorType;
    typedef typename SubOperatorType::ValueType ValueType;
    typedef typename
    ASubArithmeticRule<typename VarType::ValueType>::ATheoremType ATheoremType;

    SubArithmeticRule(const std::string& name_, const std::shared_ptr<VarType>& var);

    size_t arity() const override final;
    ValueType evaluate() const override final;

    //internal methods
    bool identifyPriv(const ptr<ATheoremType>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;

    const VarType& getSon() const;

    void operator=(const ValueType& val) const;

    bool operator==(const ASubArithmeticRule<ValueType>& prop) const override final;
    bool operator==(const ASubArithmeticTheorem<ValueType>& prop) const override final;
    bool operator==(const SubArithmeticRule& prop) const;
    bool operator==(const SubArithmeticTheorem<SubOperatorType>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;

    ~SubArithmeticRule() override = default;

    const std::shared_ptr<SubOperatorType> m_son;
};

template<typename VarType>
SubArithmeticRule<VarType>::SubArithmeticRule(const std::string &name_, const std::shared_ptr<VarType> &var):
    ASubArithmeticRule<typename VarType::ValueType>(name_,ToArithType<ValueType>::convert()),
    m_son(var)
{
    static_assert (!VarType::isConstantExpr(),"VarType must be Variable type in ConstSubArithmeticRule");
    static_assert(!std::is_same_v<VarType,Boolean >,"VarType cannot be Boolean, use SubRule<Boolean> instead");
}

template<typename VarType>
size_t SubArithmeticRule<VarType>::arity() const
{
    return 0;
}

template<typename VarType>
typename SubArithmeticRule<VarType>::ValueType SubArithmeticRule<VarType>::evaluate() const
{
    return m_son->evaluate();
}


template<typename VarType>
bool SubArithmeticRule<VarType>::
identifyPriv(const ptr<typename SubArithmeticRule<VarType>::ATheoremType> &prop, DbVarProp &dbVarProp) const
{
    if(!dbVarProp.contains(m_son->name()))
    {
        dbVarProp[m_son->name()]=prop;
        return true;
    }
    else
    {
        auto exProp=dbVarProp[m_son->name()];
        auto exPropCast=std::dynamic_pointer_cast<const ASubArithmeticRule<ValueType>>(exProp);
        if(exPropCast)
        {
            return *(prop)==*exPropCast;
        }
        else
        {
            return false;
        }

    }
}

template<typename VarType>
ptr<typename SubArithmeticRule<VarType>::ATheoremType>
SubArithmeticRule<VarType>::
applyPriv(const std::string &/*thName*/, DbVarProp &dbVarProp) const
{
    return std::dynamic_pointer_cast<const ATheoremType>(dbVarProp[m_son->name()]);
}


template<typename VarType>
ptr<typename SubArithmeticRule<VarType>::ATheoremType>
SubArithmeticRule<VarType>::applyFirstPriv(const std::string &/*thName*/, DbVarProp &dbVarProp) const
{
    return std::dynamic_pointer_cast<const ATheoremType>(dbVarProp[m_son->name()]);
}


template<typename VarType>
const VarType &SubArithmeticRule<VarType>::getSon() const
{
    return *(m_son.get());
}

template<typename VarType>
void SubArithmeticRule<VarType>::operator=(const ValueType &val) const
{
    *m_son=val;
}

template<typename VarType>
bool SubArithmeticRule< VarType>::
operator==(const ASubArithmeticTheorem<ValueType>& prop) const
{
    auto propCast=dynamic_cast<const SubArithmeticTheorem<VarType>*>(&prop);
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
bool SubArithmeticRule< VarType>::
operator==(const ASubArithmeticRule<ValueType>& prop) const
{
    auto propCast=dynamic_cast<const SubArithmeticRule*>(&prop);
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
bool SubArithmeticRule<VarType>::
operator==(const SubArithmeticRule<VarType> &prop) const
{
    return *m_son==(prop.getSon());
}

template<typename VarType>
bool SubArithmeticRule<VarType>::operator==(const SubArithmeticTheorem<VarType> &prop) const
{
    return *m_son==(prop.getSon());
}

template<typename VarType>
std::string SubArithmeticRule<VarType>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}
}

#endif // VARIABLERULEFORMULA_H
