/*===--- iisubtheoremformula.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines IISubTheoremFormula in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_IISUBTHEOREMFORMULA_H
#define DARK_LOGIC_IISUBTHEOREMFORMULA_H
#include "Utils/utils.h"

namespace N_DarkLogic
{
class IISubTheoremFormula
{
public:
    enum TheoremFormulaType
    {
        TERMINAL,
        SUBPURETH,
        SUBIMPURETH,
        NONTERMARITH
    };

    IISubTheoremFormula() = default;

    virtual TheoremFormulaType getFormulaType() const;

    virtual ~IISubTheoremFormula() = default;
};
}
#endif // DARK_LOGIC_IISUBTHEOREMFORMULA_H
