/*===--- asubimpuretheorem.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines ASubImpureTheorem for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_ASUBIMPURETHEOREM_H
#define DARK_LOGIC_ASUBIMPURETHEOREM_H
#include "asubtheorem.h"

namespace N_DarkLogic
{

class ASubImpureTheorem: public ASubTheorem
{
public:
    ASubImpureTheorem() = default;

    virtual const ptr<IISubTheoremFormula>& operator[](const size_t& index) const =0;
    TheoremFormulaType getFormulaType() const override final;

    ~ASubImpureTheorem() override = default;
};
}
#endif // DARK_LOGIC_ASUBIMPURETHEOREM_H
