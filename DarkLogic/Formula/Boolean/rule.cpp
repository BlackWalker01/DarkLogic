/*===--- rule.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements Rule class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "rule.h"
#include "Operator/operator.h"
#include "Formula/dbvarprop.h"
//#include "logichash.h"
#include "Variable/Boolean/boolean.h"
#include "Variable/dbvar.h"
#include "theorem.h"
#include "Formula/Arithmetic/allarithformula.h"

using namespace N_DarkLogic;

template<>
inline bool Rule<Equivalent<ASubRule>>::isSymetric() const
{
    return true;
}