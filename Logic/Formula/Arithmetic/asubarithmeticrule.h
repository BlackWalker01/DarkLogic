#ifndef ASUBARITHMETICRULE_H
#define ASUBARITHMETICRULE_H
#include "Formula/Arithmetic/aarithmeticformula.h"
#include "Formula/isubruleformula.h"

namespace N_Logic
{
template<typename ValueType>
class ASubArithmeticRule: public AArithMeticFormula<ValueType>, public ISubRuleFormula<ASubArithmeticTheorem<ValueType>>
{
public:
    ASubArithmeticRule(const std::string& name_, const ArithType type_);
    ASubArithmeticRule(const std::string& name_, const DbVar& dbVar, const ArithType type_);
    ASubArithmeticRule(const std::string& name_, const DbVar& dbVar, const DbVar& dbVar2, const ArithType type_);
    ASubArithmeticRule(const std::string& name_, const std::vector<DbVar>& dbVars, const ArithType type_);

    virtual ~ASubArithmeticRule() = default;
};

template<typename ValueType>
ASubArithmeticRule<ValueType>::ASubArithmeticRule(const std::string &name_, const ArithType type_):
    AArithMeticFormula<ValueType> (name_,type_)
{

}

template<typename ValueType>
ASubArithmeticRule<ValueType>::ASubArithmeticRule(const std::string &name_, const std::vector<DbVar> &dbVars, const ArithType type_):
    AArithMeticFormula<ValueType>(name_,dbVars,type_)
{

}

template<typename ValueType>
ASubArithmeticRule<ValueType>::ASubArithmeticRule(const std::string &name_, const DbVar &dbVar, const ArithType type_):
    AArithMeticFormula<ValueType>(name_,dbVar,type_)
{

}

template<typename ValueType>
ASubArithmeticRule<ValueType>::ASubArithmeticRule(const std::string &name_, const DbVar &dbVar, const DbVar &dbVar2, const ArithType type_):
    AArithMeticFormula<ValueType>(name_,dbVar,dbVar2,type_)
{

}
}
#endif // ASUBARITHMETICRULE_H
