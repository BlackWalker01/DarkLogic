#include "naturalintegerset.h"
#include "set.h"

using namespace N_Logic;

//template<> const std::unique_ptr<AxiomSet<NaturalIntegerSet>> Set<NaturalIntegerSet>::s_axiomSet=nullptr;
template<> const std::unique_ptr<DbRule> Set<NaturalIntegerSet>::s_rules = std::make_unique<DbRule>();

template<> template<RuleType ruleType>
void Set<NaturalIntegerSet>::insert(const ptr<ruleType >& rule)
{
    s_rules->insert(rule);
}

