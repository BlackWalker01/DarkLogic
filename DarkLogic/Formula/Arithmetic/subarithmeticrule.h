/*===--- subarithmeticrule.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file pre-defines SubArithmeticRule in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SUBARITHMETICRULE_H
#define DARK_LOGIC_SUBARITHMETICRULE_H
#include "Formula/Arithmetic/asubarithmeticrule.h"
#include "Operator/Arithmetic/allarithmeticoperator.h"
#include "Variable/term.h"
#include "subarithmetictheorem.h"
#include "Variable/dbvar.h"
#include "Variable/allvariable.h"

namespace N_DarkLogic
{

template<SubRuleFormula SubFormulaType>
class SubArithmeticRule;

template<typename ValueType>
bool operator==(const ASubArithmeticRule<ValueType>& rule, const ASubArithmeticTheorem<ValueType>& thm)
{
	return rule.isEqual(thm);
}

template<typename ValueType>
bool operator==(const ASubArithmeticTheorem<ValueType>& th, const ASubArithmeticTheorem<ValueType>& th2)
{
	return th.isEqual(th2);
}

template<typename ValueType>
bool operator==(const ASubArithmeticRule<ValueType>& th, const ASubArithmeticRule<ValueType>& th2)
{
	return th.isEqual(th2);
}

}
#endif // DARK_LOGIC_SUBARITHMETICRULE_H
