/*===--- axiom.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines Axiom class in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_AXIOM_H
#define DARK_LOGIC_AXIOM_H
#include "rule.h"

namespace N_DarkLogic {
template<typename SubPropertyType>
class Axiom: public Rule<SubPropertyType>
{
public:
    Axiom(const std::string& name_, const ptr<ASubRule>& leftSubProperty, const ptr<ASubRule>& rightSubProperty);
};

ptr<ASubRule> createAxiom(const std::string& name, const std::string& content);
ptr<ASubRule> createAxiom(const std::string &name, std::vector<OperatorOrdering> &opeList,
                          std::vector<OperatorOrdering> &orderedOpeList,
                           std::vector<std::shared_ptr<VariableContainer> > &varList);

template<typename SubPropertyType>
Axiom<SubPropertyType>::Axiom(const std::string &name_, const ptr<ASubRule> &leftSubProperty, const ptr<ASubRule> &rightSubProperty):
    Rule<SubPropertyType>(name_,leftSubProperty,rightSubProperty)
{

}

}
#endif // DARK_LOGIC_AXIOM_H
