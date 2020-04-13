/*===--- isubtheoremformula.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines ISubTheoremFormula in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/


#ifndef DARK_LOGIC_ISUBTHEOREMFORMULA_H
#define DARK_LOGIC_ISUBTHEOREMFORMULA_H
#include "iisubtheoremformula.h"

namespace N_DarkLogic {

template<typename ATheoremType_>
class ISubTheoremFormula: public IISubTheoremFormula
{
public:
    typedef ATheoremType_ ATheoremType;
    ISubTheoremFormula() = default;

    virtual ~ISubTheoremFormula() = default;
};
}
#endif // DARK_LOGIC_ISUBTHEOREMFORMULA_H
