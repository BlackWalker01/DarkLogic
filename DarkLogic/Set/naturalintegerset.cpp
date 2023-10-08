/*===--- naturalintegerset.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements natural integer set class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "naturalintegerset.h"
#include "set.h"

using namespace N_DarkLogic;

//template<> const std::unique_ptr<AxiomSet<NaturalIntegerSet>> Set<NaturalIntegerSet>::s_axiomSet=nullptr;
template<> const std::unique_ptr<DbRule> Set<NaturalIntegerSet>::s_rules = std::make_unique<DbRule>();
