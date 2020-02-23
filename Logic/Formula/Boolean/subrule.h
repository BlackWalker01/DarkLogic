#ifndef SUBRULE_H
#define SUBRULE_H
#include "asubrule.h"
#include "theorem.h"
#include "Set/booleanset.h"
#include "Variable/dbvar.h"
#include "Formula/dbvarprop.h"

namespace N_Logic
{

template<typename SubPropertyType>
class SubRule;

ptr<ValueTypeObject> createSubRule(const std::string &name, std::vector<OperatorOrdering> &opeList,
                                      std::vector<OperatorOrdering> &orderedOpeList,
                                   std::vector<std::shared_ptr<VariableContainer> > &varList);

}

#endif // SUBRULE_H
