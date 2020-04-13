/*===--- theorem.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file pre-defines Theorem class in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_THEOREM_H
#define DARK_LOGIC_THEOREM_H
#include "allsubtheorem.h"
#include "Utils/utils.h"

namespace N_DarkLogic{

template<SubTheoremProperty SubPropertyType>
class Theorem;

ptr<ASubTheorem> createTheorem(const std::string &name, const std::string &content);

ptr<ValueTypeObject> createTheorem(const std::string &name, std::vector<OperatorOrdering> &opeList,
                               std::vector<OperatorOrdering> &orderedOpeList,
                                   std::vector<std::shared_ptr<VariableContainer>> &varList);

}
#endif // DARK_LOGIC_THEOREM_H
