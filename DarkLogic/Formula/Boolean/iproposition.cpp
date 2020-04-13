/*===--- iproposition.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements IProposition class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "iproposition.h"
#include "asubrule.h"
#include "asubtheorem.h"

using namespace N_DarkLogic;

IDVar N_DarkLogic::IProposition::variableId() const
{
	throw std::logic_error("variableId() can only be called by SubRule<Boolean> or SubTheorem<Boolean>");
}
