#ifndef SUBRULE_H
#define SUBRULE_H
#include "asubrule.h"
#include "theorem.h"
#include "Set/booleanset.h"
#include "Variable/dbvar.h"
#include "Formula/dbvarprop.h"

namespace N_Logic
{
template<SubRuleProperty SubPropertyType>
class SubRule;

ptr<ValueTypeObject> createSubRule(const std::string &name, std::vector<OperatorOrdering> &opeList,
                                      std::vector<OperatorOrdering> &orderedOpeList,
                                   std::vector<std::shared_ptr<VariableContainer> > &varList);

bool operator==(const ASubRule& rule, const ASubTheorem& thm)
{
    return rule.isEqual(thm);
}

bool operator==(const ASubTheorem& th, const ASubTheorem& th2)
{
    return th.isEqual(th2);
}

bool operator==(const ASubRule& th, const ASubRule& th2)
{
    return th.isEqual(th2);
}

}

#endif // SUBRULE_H
