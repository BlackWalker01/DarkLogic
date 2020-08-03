/*===--- subtheorem.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file pre-sdefines SubTheorem class in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SUBTHEOREM_H
#define DARK_LOGIC_SUBTHEOREM_H
#include "asubtheorem.h"
#include "Set/booleanset.h"
#include "Variable/dbvar.h"
#include "Operator/alloperator.h"
#include "Variable/Boolean/boolean.h"
#include "Variable/Boolean/constboolean.h"
#include "Formula/dbvarprop.h"

namespace N_DarkLogic
{
template<SubRuleProperty SubPropertyType>
class SubRule;

template<SubTheoremProperty SubPropertyType>
class SubTheorem;

ptr<ValueTypeObject> createSubTheorem(const std::string &name, std::vector<OperatorOrdering> &opeList,
                                  std::vector<OperatorOrdering> &orderedOpeList, std::vector<std::shared_ptr<VariableContainer> > &varList,
                                    const size_t& nbHyps = 0);
}

#endif // DARK_LOGIC_SUBTHEOREM_H
