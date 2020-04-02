#ifndef SET_IMPL_H
#define SET_IMPL_H
#include "set.h"
#include "Formula/Boolean/dbrule.h"

namespace N_Logic
{
template<typename SetType_> template<SubRuleProperty SubPropertyType>
void Set<SetType_>::insert(const ptr<Rule<SubPropertyType> > &rule)
{
    s_rules->insert(rule);
}
}

#endif // SET_IMPL_H
