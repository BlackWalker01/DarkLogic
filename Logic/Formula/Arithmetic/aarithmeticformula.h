#ifndef AARITHMETICFORMULA_H
#define AARITHMETICFORMULA_H
#include "Formula/formula.h"

namespace N_Logic
{

enum ArithType
{
    PLUS_FORMULA,
    MINUS_FORMULA,
    MULT_FORMULA,
    DIV_FORMULA,
    NAT_INT_FORMULA,
    NAT_INT_SET_FORMULA,
    BOOL_SET_FORMULA
};

template<typename ValueType>
class ASubArithmeticRule;

template<typename ValueType>
class ASubArithmeticTheorem;

template<typename ValueType>
class AArithMeticFormula: public AbstractFormula<ValueType>
{
public:
    AArithMeticFormula(const std::string& name_, const ArithType type_);
    AArithMeticFormula(const std::string& name_, const DbVar& dbVar, const ArithType type_);
    AArithMeticFormula(const std::string& name_, const DbVar& dbVar, const DbVar& dbVar2, const ArithType type_);
    AArithMeticFormula(const std::string& name_, const std::vector<DbVar>& dbVars, const ArithType type_);

    ArithType type() const;


    //method to get sub properties
    virtual size_t arity() const =0;
    //virtual DbVar<BooleanSet>& getExtVars()=0;

    virtual bool isEqual(const ASubArithmeticRule<ValueType>& prop) const =0;
    virtual bool isEqual(const ASubArithmeticTheorem<ValueType>& prop) const =0;

    virtual ~AArithMeticFormula() = default;
protected:
    const ArithType m_type;
};

template<typename ValueType>
inline bool operator==(const ASubArithmeticRule<ValueType>& rule, const ASubArithmeticTheorem<ValueType>& thm);
template<typename ValueType>
inline bool operator==(const ASubArithmeticTheorem<ValueType>& th, const ASubArithmeticTheorem<ValueType>& th2);
template<typename ValueType>
inline bool operator==(const ASubArithmeticRule<ValueType>& th, const ASubArithmeticRule<ValueType>& th2);

template<typename ValueType>
struct ToArithType
{
    static ArithType convert();
};

template<typename ValueType>
AArithMeticFormula<ValueType>::AArithMeticFormula(const std::string& name_, const ArithType type_):
    AbstractFormula<ValueType>(name_), m_type(type_)
{

}

template<typename ValueType>
AArithMeticFormula<ValueType>::AArithMeticFormula(const std::string &name_, const std::vector<DbVar> &dbVars, const ArithType type_):
    AbstractFormula<ValueType>(name_,dbVars), m_type(type_)
{

}

template<typename ValueType>
AArithMeticFormula<ValueType>::AArithMeticFormula(const std::string &name_, const DbVar &dbVar, const ArithType type_):
    AbstractFormula<ValueType>(name_,dbVar), m_type(type_)
{

}

template<typename ValueType>
AArithMeticFormula<ValueType>::AArithMeticFormula(const std::string &name_, const DbVar &dbVar, const DbVar &dbVar2, const ArithType type_):
    AbstractFormula<ValueType>(name_,dbVar,dbVar2), m_type(type_)
{

}

template<typename ValueType>
ArithType AArithMeticFormula<ValueType>::type() const
{
    return m_type;
}
}
#endif // AARITHMETICFORMULA_H
