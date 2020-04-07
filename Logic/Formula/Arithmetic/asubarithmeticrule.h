#ifndef ASUBARITHMETICRULE_H
#define ASUBARITHMETICRULE_H
#include "Formula/Arithmetic/aarithmeticformula.h"
#include "Formula/isubruleformula.h"

namespace N_Logic
{
template<ArithmeticType ValueType>
class ASubArithmeticRule: public AArithMeticFormula<ValueType>, public ISubRuleFormula<ASubArithmeticTheorem<ValueType>>
{
public:
    ASubArithmeticRule() = default;

    virtual ~ASubArithmeticRule() = default;
};
template<ArithmeticType ValueType> struct ToTheoremStruct<ASubArithmeticRule<ValueType>> { using Type = ASubArithmeticTheorem<ValueType>; };
}
#endif // ASUBARITHMETICRULE_H
