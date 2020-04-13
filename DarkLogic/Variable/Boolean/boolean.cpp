/*===--- boolean.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements boolean variables for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "boolean.h"

using namespace N_DarkLogic;

Boolean::Boolean(const std::string& name_): Variable<BooleanSet>(name_)
{

}

VALUE_TYPE Boolean::valueType() const
{
    return VALUE_TYPE::BOOL_TYPE;
}

void N_DarkLogic::Boolean::operator=(const ValueType& val) const
{
    (*m_val) = std::make_unique<bool>(val);
}

Boolean::~Boolean()
{

}
