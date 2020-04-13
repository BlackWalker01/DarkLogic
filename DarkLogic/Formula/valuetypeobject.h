/*===--- valuetypeobject.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines ValueTypeObject in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_VALUETYPEOBJECT_H
#define DARK_LOGIC_VALUETYPEOBJECT_H
#include <memory>
#include <sstream>

namespace N_DarkLogic {

enum VALUE_TYPE
{
    VOID_TYPE,
    BOOL_TYPE,
    NATURAL_INT_TYPE,
    BOOLEANSET_TYPE,
    NATURAL_INTEGERSET_TYPE
};

class ValueTypeObject
{
public:
    ValueTypeObject() = default;

    virtual VALUE_TYPE valueType() const=0;

    virtual ~ValueTypeObject() = default;
};
}
#endif // DARK_LOGIC_VALUETYPEOBJECT_H
