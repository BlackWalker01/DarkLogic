#include "asubpurerule.h"

using namespace N_Logic;

ASubPureRule::ASubPureRule(const std::string &name_, const N_Logic::IProposition::PropType type_):
    ASubRule(name_,type_)
{

}

ASubPureRule::ASubPureRule(const std::string &name_, const DbVar &dbVar, const IProposition::PropType type_):
    ASubRule(name_,dbVar,type_)
{

}

ASubPureRule::ASubPureRule(const std::string &name_, const DbVar &dbVar, const DbVar &dbVar2, const IProposition::PropType type_):
    ASubRule(name_,dbVar,dbVar2,type_)
{

}

ASubPureRule::ASubPureRule(const std::string &name_, const std::vector<DbVar> &dbVars, const IProposition::PropType type_):
    ASubRule(name_,dbVars,type_)
{

}
