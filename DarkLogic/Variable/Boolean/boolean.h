/*===--- boolean.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines boolean variable in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_BOOLEAN_H
#define DARK_LOGIC_BOOLEAN_H
#include "Variable/variable.h"
#include "Set/booleanset.h"

namespace N_DarkLogic {
class Boolean : public Variable<BooleanSet>
{
public:
    Boolean(const std::string& name_);

    VALUE_TYPE valueType() const override final;
    void operator=(const ValueType& val) const;

    ~Boolean() override;
};
}
#endif // DARK_LOGIC_BOOLEAN_H
