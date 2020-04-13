/*===--- naturalintegerset.h - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines natural integer set in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef NATURALINTEGERSET_H
#define NATURALINTEGERSET_H
#include "set.h"

namespace N_DarkLogic {

class NaturalIntegerSet: public Set<NaturalIntegerSet>
{
public:
    NaturalIntegerSet() = default;
    using Type=UInt;

    ~NaturalIntegerSet() = default;
};
}
#endif // NATURALINTEGERSET_H
