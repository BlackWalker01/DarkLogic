#ifndef ASUBARITHMETICTHEOREM_H
#define ASUBARITHMETICTHEOREM_H
#include "aarithmeticformula.h"
#include "Formula/isubtheoremformula.h"

namespace N_Logic
{
typedef size_t Arity;

template<typename ValueType>
class ASubArithmeticTheorem: public AArithMeticFormula<ValueType>, public ISubTheoremFormula<ASubArithmeticTheorem<ValueType>>
{
public:
    ASubArithmeticTheorem(const std::string &name_, const ArithType type_);
    ASubArithmeticTheorem(const std::string& name_, const DbVar& dbVar, const ArithType type_);
    ASubArithmeticTheorem(const std::string& name_, const DbVar& dbVar, const DbVar& dbVar2, const ArithType type_);
    ASubArithmeticTheorem(const std::string& name_, const std::vector<DbVar>& dbVars, const ArithType type_);
    ASubArithmeticTheorem(const ASubArithmeticTheorem& formula) = delete;

    const std::vector<std::vector<Arity> > &getAllPaths() const;
    virtual const std::vector<std::vector<Arity>>& computeAllPaths()=0;

    virtual ~ASubArithmeticTheorem() = default;

protected:
    std::vector<std::vector<Arity>> m_allPaths;
};

template<typename ValueType>
ASubArithmeticTheorem<ValueType>::ASubArithmeticTheorem(const std::string& name_, const ArithType type_):
    AArithMeticFormula<ValueType> (name_,type_)
{

}

template<typename ValueType>
ASubArithmeticTheorem<ValueType>::ASubArithmeticTheorem(const std::string &name_, const std::vector<DbVar> &dbVars, const ArithType type_):
    AArithMeticFormula<ValueType>(name_,dbVars,type_)
{

}

template<typename ValueType>
ASubArithmeticTheorem<ValueType>::ASubArithmeticTheorem(const std::string &name_, const DbVar &dbVar, const ArithType type_):
    AArithMeticFormula<ValueType>(name_,dbVar,type_)
{

}

template<typename ValueType>
ASubArithmeticTheorem<ValueType>::ASubArithmeticTheorem(const std::string &name_, const DbVar &dbVar, const DbVar &dbVar2, const ArithType type_):
    AArithMeticFormula<ValueType>(name_,dbVar,dbVar2,type_)
{

}

template<typename ValueType>
const std::vector<std::vector<Arity> > &ASubArithmeticTheorem<ValueType>::getAllPaths() const
{
    return m_allPaths;
}
}
#endif // ASUBARITHMETICTHEOREM_H
