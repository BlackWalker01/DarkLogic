/*===--- asubarithmeticrule.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines ASubArithmeticRule in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_ASUBARITHMETICRULE_H
#define DARK_LOGIC_ASUBARITHMETICRULE_H
#include "Formula/Arithmetic/aarithmeticformula.h"
#include "Formula/isubruleformula.h"

namespace N_DarkLogic
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
#endif // DARK_LOGIC_ASUBARITHMETICRULE_H
