/*===--- asubpuretheorem.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines ASubPureTheorem class in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_ASUBPURETHEOREM_H
#define DARK_LOGIC_ASUBPURETHEOREM_H
#include "asubtheorem.h"

namespace N_DarkLogic
{
class ASubPureTheorem: public ASubTheorem
{
public:
    ASubPureTheorem() = default;


    virtual const ptr<ASubTheorem>& operator[](const size_t& index) const =0;
    TheoremFormulaType getFormulaType() const override final;

    virtual ~ASubPureTheorem() = default;
};
}

#endif // ASUBPURETHEOREM_H
