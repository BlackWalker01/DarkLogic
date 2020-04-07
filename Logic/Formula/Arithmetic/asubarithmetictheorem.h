#ifndef ASUBARITHMETICTHEOREM_H
#define ASUBARITHMETICTHEOREM_H
#include "aarithmeticformula.h"
#include "Formula/isubtheoremformula.h"

namespace N_Logic
{
typedef size_t Arity;

template<ArithmeticType ValueType>
class ASubArithmeticTheorem: public AArithMeticFormula<ValueType>, public ISubTheoremFormula<ASubArithmeticTheorem<ValueType>>
{
public:
    ASubArithmeticTheorem() = default;
    ASubArithmeticTheorem(const ASubArithmeticTheorem& formula) = delete;

    const std::vector<std::vector<Arity> > &getAllPaths() const;
    virtual const std::vector<std::vector<Arity>>& computeAllPaths()=0;

    virtual ~ASubArithmeticTheorem() = default;

protected:
    std::vector<std::vector<Arity>> m_allPaths;
};

template<ArithmeticType ValueType> struct ToRuleStruct<ASubArithmeticTheorem<ValueType>> { using Type = ASubArithmeticRule<ValueType>; };

template<ArithmeticType ValueType>
const std::vector<std::vector<Arity> > &ASubArithmeticTheorem<ValueType>::getAllPaths() const
{
    return m_allPaths;
}
}
#endif // ASUBARITHMETICTHEOREM_H
