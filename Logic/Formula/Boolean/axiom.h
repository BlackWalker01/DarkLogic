#ifndef AXIOM_H
#define AXIOM_H
#include "rule.h"

namespace N_Logic {
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
#endif // AXIOM_H
