/*===--- asubpurerule.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines ASubPureRule DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_ASUBPURERULE_H
#define DARK_LOGIC_ASUBPURERULE_H
#include "asubrule.h"

namespace N_DarkLogic
{
class ASubPureRule: public ASubRule
{
public:
    ASubPureRule() = default;

    virtual const ptr<ASubRule>& operator[](const size_t& index) const =0;

    virtual ~ASubPureRule() override = default;
};
}
#endif // DARK_LOGIC_ASUBPURERULE_H
