#include "aarithmeticformula.h"
#include "Variable/allvariable.h"

using namespace N_Logic;

template<>
ArithType ToArithType<size_t>::convert()
{
    return NAT_INT_FORMULA;
}

template<>
ArithType ToArithType<NaturalIntegerSet>::convert()
{
    return NAT_INT_SET_FORMULA;
}

template<>
ArithType ToArithType<BooleanSet>::convert()
{
    return BOOL_SET_FORMULA;
}
