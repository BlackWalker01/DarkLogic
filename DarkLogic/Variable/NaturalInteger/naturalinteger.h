/*===--- naturalinteger.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines natural integers in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_NATURALINTEGER_H
#define DARK_LOGIC_NATURALINTEGER_H
#include "Variable/variable.h"
#include "Set/naturalintegerset.h"

namespace N_DarkLogic {
class NaturalInteger: public Variable<NaturalIntegerSet>
{
public:
    NaturalInteger(const std::string& name_);

    VALUE_TYPE valueType() const override final;
    void operator=(const ValueType& val);

    ~NaturalInteger() override;
};
}

#endif // DARK_LOGIC_NATURALINTEGER_H
