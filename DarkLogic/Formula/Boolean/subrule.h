/*===--- subrule.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file pre-defines SubRule class in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SUBRULE_H
#define DARK_LOGIC_SUBRULE_H
#include "asubrule.h"
#include "theorem.h"
#include "Set/booleanset.h"
#include "Variable/dbvar.h"
#include "Formula/dbvarprop.h"

namespace N_DarkLogic
{
template<SubRuleProperty SubPropertyType>
class SubRule;

bool operator==(const ASubRule& rule, const ASubTheorem& thm)
{
    return rule.isEqual(thm);
}

bool operator==(const ASubTheorem& th, const ASubTheorem& th2)
{
    return th.isEqual(th2);
}

bool operator==(const ASubRule& th, const ASubRule& th2)
{
    return th.isEqual(th2);
}

}

#endif // DARK_LOGIC_SUBRULE_H
