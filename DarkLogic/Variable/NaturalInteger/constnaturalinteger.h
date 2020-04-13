/*===--- constnaturalinteger.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines natural integer constants in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_CONSTNATURALINTEGER_H
#define DARK_LOGIC_CONSTNATURALINTEGER_H
#include "Variable/constante.h"
#include "Set/naturalintegerset.h"

namespace N_DarkLogic {
class ConstNaturalInteger: public Constante<NaturalIntegerSet>
{
public:
    ConstNaturalInteger(const typename NaturalIntegerSet::Type& val);

    VALUE_TYPE valueType() const override final;
    std::string toString(unsigned short =1000) const;

    ~ConstNaturalInteger() override = default;
};
}

#endif // DARK_LOGIC_CONSTNATURALINTEGER_H
