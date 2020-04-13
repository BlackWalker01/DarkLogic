/*===--- asubimpurerule.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines ASubImpureRule class in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_ASUBIMPURERULE_H
#define DARK_LOGIC_ASUBIMPURERULE_H
#include "asubrule.h"

namespace N_DarkLogic
{
class ASubImpureRule: public ASubRule
{
public:
    ASubImpureRule() = default;

    virtual const ptr<IISubRuleFormula>& operator[](const size_t& index) const =0;

    ~ASubImpureRule() override = default;
};
}
#endif // ASUBIMPURERULE_H
