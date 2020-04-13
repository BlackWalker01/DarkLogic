/*===--- booleanset.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines boolean set in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_BOOLEANSET_H
#define DARK_LOGIC_BOOLEANSET_H
#include "set.h"

namespace N_DarkLogic {

class BooleanSet: public Set<BooleanSet>
{
public:
    BooleanSet() = default;
    using Type=bool;

    void init();

    ~BooleanSet() = default;
};
}

#endif // DARK_LOGIC_BOOLEANSET_H
