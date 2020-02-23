#ifndef SUBTHEOREM_H
#define SUBTHEOREM_H
#include "asubtheorem.h"
#include "Set/booleanset.h"
#include "Variable/dbvar.h"
#include "Operator/alloperator.h"
#include "Variable/Boolean/boolean.h"
#include "Variable/Boolean/constboolean.h"
#include "Formula/dbvarprop.h"

namespace N_Logic
{

template<typename SubPropertyType>
class SubRule;

template<typename SubPropertyType>
class SubTheorem;

ptr<ValueTypeObject> createSubTheorem(const std::string &name, std::vector<OperatorOrdering> &opeList,
                                  std::vector<OperatorOrdering> &orderedOpeList, std::vector<std::shared_ptr<VariableContainer> > &varList);
#include "subtheorem.hxx"
}

#endif // SUBTHEOREM_H
