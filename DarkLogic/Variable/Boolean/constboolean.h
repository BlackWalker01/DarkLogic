/*===--- constboolean.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines boolean constant in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef CONST_BOOLEAN_H
#define CONST_BOOLEAN_H

#include "Variable/constante.h"
#include "Set/booleanset.h"
#include <sstream>

namespace N_DarkLogic {
class ConstBoolean: public Constante<BooleanSet>
{
public:
    ConstBoolean(const bool& val);

    VALUE_TYPE valueType() const override final;
    std::string toString(unsigned short =1000) const;

    ~ConstBoolean() = default;
};
}
#endif // CONST_BOOLEAN_H
