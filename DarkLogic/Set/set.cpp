/*===--- set.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements set class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "set.h"
#include "Formula/Boolean/dbrule.h"
#include "Set/allset.h"

using namespace N_DarkLogic;

//Set initialization
template<> const std::string Set<BooleanSet>::s_name = "BB";
template<> const std::string Set<NaturalIntegerSet>::s_name = "NN";

template<>
std::string Set<BooleanSet>::name()
{
    return s_name;
}

template<>
std::string Set<NaturalIntegerSet>::name()
{
    return s_name;
}
