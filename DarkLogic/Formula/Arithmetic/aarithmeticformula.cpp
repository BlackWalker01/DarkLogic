/*===--- aarithmeticformula.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements aarithmeticformula class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "aarithmeticformula.h"
#include "Variable/allvariable.h"

using namespace N_DarkLogic;

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
