/*===--- constnaturalinteger.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements ConstNaturalInteger class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "constnaturalinteger.h"

using namespace N_DarkLogic;

ConstNaturalInteger::ConstNaturalInteger(const typename NaturalIntegerSet::Type &val)
    : Constante<NaturalIntegerSet>(val)
{

}

VALUE_TYPE ConstNaturalInteger::valueType() const
{
    return VALUE_TYPE::NATURAL_INT_TYPE;
}

std::string ConstNaturalInteger::toString(unsigned short) const
{
    return m_name;
}
