/*===--- constset.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements set constants for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "constset.h"

using namespace N_DarkLogic;

template<>
VALUE_TYPE ConstSet<NaturalIntegerSet>::valueType() const
{
    return NATURAL_INTEGERSET_TYPE;
}

template<>
VALUE_TYPE ConstSet<BooleanSet>::valueType() const
{
    return BOOLEANSET_TYPE;
}
