#ifndef SETEQUALRULEFORMULA_H
#define SETEQUALRULEFORMULA_H
#include "Formula/Arithmetic/subarithmeticrule.h"
#include "Operator/Arithmetic/setequal.h"
#include "Formula/Arithmetic/inontermarithmetic.h"

namespace N_Logic
{

template<typename VariableType_>
class SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType_>>>:
        public ASubArithmeticRule<void>,
        public INonTermArithmetic<IISubRuleFormula>
{
public:
    typedef SubArithmeticRule<VariableType_> SubLeftFormula;
    typedef ASubArithmeticRule<typename VariableType_::ValueType> SubRightFormula;
    typedef SetEqual<SubArithmeticRule<VariableType_>> SubOperatorType;
    typedef void ValueType;
    typedef ASubArithmeticTheorem<void> ATheoremType;

    SubArithmeticRule(const ptr<SubLeftFormula>& leftFormula,
                      const ptr<SubRightFormula>& rightFormula);

    size_t arity() const override final;
    ValueType evaluate() const override final;
    constexpr ArithType type() const override final;

    //internal methods
    bool identifyPriv(const ptr<ATheoremType>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp) const override final;
    ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp) const override final;

    bool isEqual(const ASubArithmeticRule<ValueType>& prop) const override final;
    bool isEqual(const ASubArithmeticTheorem<ValueType>& prop) const override final;
    bool operator==(const SubArithmeticRule& prop) const;
    bool operator==(const SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType_>>>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;

    const SubOperatorType& getSon() const;
    const ptr<IISubRuleFormula>& operator[](const size_t& k) const override final;

    ~SubArithmeticRule() override = default;

protected:
    const std::unique_ptr<const SubOperatorType> m_son;
    const DbVar m_extVars;
};

template<typename VariableType>
SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::
SubArithmeticRule(const ptr<SubArithmeticRule<VariableType>> &leftFormula,
                  const ptr<ASubArithmeticRule<typename VariableType::ValueType>> &rightFormula):
    m_son(std::make_unique<SetEqual<SubArithmeticRule<VariableType>>>(leftFormula,rightFormula)),
    m_extVars(leftFormula->getExtVars(), rightFormula->getExtVars())
{

}

template<typename VariableType>
constexpr ArithType SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::type() const
{
    return SET_FORMULA;
}


template<typename VariableType>
size_t SubArithmeticRule< SetEqual<SubArithmeticRule<VariableType>> >::arity() const
{
    return m_son->arity();
}


template<typename VariableType>
bool SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::
isEqual(const ASubArithmeticRule<void>& prop) const
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

template<typename VariableType>
bool SubArithmeticRule< SetEqual<SubArithmeticRule<VariableType>> >::
isEqual(const ASubArithmeticTheorem<ValueType>& prop) const
{
    auto propCast=
            dynamic_cast<const SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>>>*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<typename VariableType>
bool SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::
operator==(const SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> > &prop) const
{
    return *m_son==(prop.getSon());
}

template<typename VariableType>
bool SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::
operator==(const SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>>> &prop) const
{
    return *m_son==prop.getSon();
}

template<typename VariableType>
bool SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::
identifyPriv(const ptr<SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>>>::ATheoremType> &prop, DbVarProp &dbVarProp) const
{
    auto propCast=std::dynamic_pointer_cast<const SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>> >>(prop);
    if(propCast)
    {
        return get<0>(*m_son)->identifyPriv(get<0>(propCast->getSon()),dbVarProp) &&
                get<1>(*m_son)->identifyPriv(get<1>(propCast->getSon()),dbVarProp);
    }
    return false;
}

template<typename VariableType>
ptr<typename SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>>>::ATheoremType>
SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::
applyPriv(DbVarProp &dbVarProp) const
{
    return std::make_shared<const SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>>>>(
    std::static_pointer_cast<const SubArithmeticTheorem<VariableType>>(get<0>(*m_son)->applyPriv(dbVarProp)),
                                                get<1>(*m_son)->applyPriv(dbVarProp));
}

template<typename VariableType>
ptr<typename SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::ATheoremType>
SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::applyFirstPriv(DbVarProp &dbVarProp) const
{
    return std::make_shared<const SubArithmeticTheorem<SetEqual<SubArithmeticTheorem<VariableType>>>>(
    std::static_pointer_cast<const SubArithmeticTheorem<VariableType>>(get<0>(*m_son)->applyPriv(dbVarProp)),
                                                                get<1>(*m_son)->applyPriv(dbVarProp));
}

template<typename VariableType>
typename SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::ValueType
SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::evaluate() const
{
    return m_son->evaluate();
}

template<typename VariableType>
std::string SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

template<typename VariableType>
const DbVar* SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::getExtVars() const
{
    return &m_extVars;
}

template<typename VariableType>
const typename SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::SubOperatorType&
SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType>> >::getSon() const
{
    return *(m_son.get());
}

template<typename VariableType_>
const ptr<IISubRuleFormula> &N_Logic::SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType_> > >::
operator[](const size_t &k) const
{
    return (*m_son)[k];
}

}


#endif // SETEQUALRULEFORMULA_H
