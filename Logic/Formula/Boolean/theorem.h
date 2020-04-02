#ifndef THEOREM_H
#define THEOREM_H
#include "allsubtheorem.h"
#include "Utils/utils.h"

namespace N_Logic{

template<SubTheoremProperty SubPropertyType>
class Theorem;

ptr<ASubTheorem> createTheorem(const std::string &name, const std::string &content);

ptr<ValueTypeObject> createTheorem(const std::string &name, std::vector<OperatorOrdering> &opeList,
                               std::vector<OperatorOrdering> &orderedOpeList,
                                   std::vector<std::shared_ptr<VariableContainer>> &varList);

}
#endif // THEOREM_H
