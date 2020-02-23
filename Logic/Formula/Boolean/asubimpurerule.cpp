#include "asubimpurerule.h"

using namespace N_Logic;

ASubImpureRule::ASubImpureRule(const std::string &name_, const PropType type_):
    ASubRule(name_,type_)
{

}

ASubImpureRule::ASubImpureRule(const std::string &name_, const DbVar &dbVar, const IProposition::PropType type_):
    ASubRule(name_,dbVar,type_)
{

}

ASubImpureRule::ASubImpureRule(const std::string &name_, const DbVar &dbVar, const DbVar &dbVar2, const IProposition::PropType type_):
    ASubRule(name_,dbVar,dbVar2,type_)
{

}

ASubImpureRule::ASubImpureRule(const std::string &name_, const std::vector<DbVar> &dbVars, const IProposition::PropType type_):
    ASubRule(name_,dbVars,type_)
{

}
