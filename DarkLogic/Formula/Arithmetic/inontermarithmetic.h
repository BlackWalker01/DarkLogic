/*===--- inontermarithmetic.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines INonTermArithmetic in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_INONTERMARITHMETIC_H
#define DARK_LOGIC_INONTERMARITHMETIC_H
#include "Utils/utils.h"

namespace N_DarkLogic
{
template<typename SubPropertyType>
class INonTermArithmetic
{
public:
    INonTermArithmetic() = default;

    virtual const ptr<SubPropertyType>& operator[](const size_t& k) const =0;

    virtual ~INonTermArithmetic() = default;
};
}

#endif // DARK_LOGIC_INONTERMARITHMETIC_H
