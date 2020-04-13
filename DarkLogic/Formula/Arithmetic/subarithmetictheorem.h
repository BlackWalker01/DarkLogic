/*===--- subarithmetictheorem.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file pre-defines SubArithmeticTheorem in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SUBARITHMETICTHEOREM_H
#define DARK_LOGIC_SUBARITHMETICTHEOREM_H
#include "Formula/Arithmetic/asubarithmetictheorem.h"
#include "Operator/Arithmetic/allarithmeticoperator.h"

namespace N_DarkLogic
{
template<SubRuleFormula SubOperatorType>
class SubArithmeticRule;

template<SubTheoremFormula SubOperatorType>
class SubArithmeticTheorem;

}
#endif // DARK_LOGIC_SUBARITHMETICTHEOREM_H
