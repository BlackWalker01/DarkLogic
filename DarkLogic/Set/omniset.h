/*===--- omniset.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines "set of set" class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_OMNISET_H
#define DARK_LOGIC_OMNISET_H
#include "set.h"

namespace N_DarkLogic {
template<SetDerived SetType_>
class OmniSet: Set<OmniSet<SetType_>>
{
public:
    using SetType=SetType_;
    using Type=SetType;

    OmniSet();
};

template<SetDerived SetType_>
OmniSet<SetType_>::OmniSet()
{

}
}

#endif // DARK_LOGIC_OMNISET_H
