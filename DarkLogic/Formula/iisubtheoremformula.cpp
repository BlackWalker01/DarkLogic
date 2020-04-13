/*===--- iisubtheoremformula.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements iisubtheoremformula class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/


#include "iisubtheoremformula.h"

using namespace N_DarkLogic;

IISubTheoremFormula::TheoremFormulaType IISubTheoremFormula::getFormulaType() const
{
    return TERMINAL;
}