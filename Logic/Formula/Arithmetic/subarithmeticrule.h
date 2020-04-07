#ifndef SUBARITHMETICRULE_H
#define SUBARITHMETICRULE_H
#include "Formula/Arithmetic/asubarithmeticrule.h"
#include "Operator/Arithmetic/allarithmeticoperator.h"
#include "Variable/term.h"
#include "subarithmetictheorem.h"
#include "Variable/dbvar.h"
#include "Variable/allvariable.h"

namespace N_Logic
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
#endif // SUBARITHMETICRULE_H
