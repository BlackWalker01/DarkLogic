/*===--- naturalinteger.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements natural integer class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "naturalinteger.h"

using namespace N_DarkLogic;

NaturalInteger::NaturalInteger(const std::string &name_): Variable<NaturalIntegerSet>(name_)
{

}

VALUE_TYPE NaturalInteger::valueType() const
{
    return VALUE_TYPE::NATURAL_INT_TYPE;
}

void NaturalInteger::operator=(const Variable::ValueType &val)
{
    *m_val=std::make_unique<ValueType>(val);
}

NaturalInteger::~NaturalInteger()
{

}
