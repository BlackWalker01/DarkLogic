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
template<SubRuleProperty SubPropertyType>
class SubRule;

template<typename T>
concept SubTheoremProperty = std::is_base_of_v<Operator<typename T::FunType>, T> || std::is_same_v<T, Boolean> || std::is_same_v<T, ConstBoolean>;

template<SubTheoremProperty SubPropertyType>
class SubTheorem;

ptr<ValueTypeObject> createSubTheorem(const std::string &name, std::vector<OperatorOrdering> &opeList,
                                  std::vector<OperatorOrdering> &orderedOpeList, std::vector<std::shared_ptr<VariableContainer> > &varList);
#include "subtheorem.hxx"
}

#endif // SUBTHEOREM_H
