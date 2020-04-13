/*===--- constboolean.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements boolean constants for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "constboolean.h"

using namespace N_DarkLogic;

ConstBoolean::ConstBoolean(const bool &val): Constante<BooleanSet>(val)
{

}

VALUE_TYPE ConstBoolean::valueType() const
{
    return VALUE_TYPE::BOOL_TYPE;
}

std::string ConstBoolean::toString(unsigned short) const
{
    return m_name;
}

template<>
std::string Constante<BooleanSet>::valToString(const bool& val)
{
    return val? "True" : "False";
}
