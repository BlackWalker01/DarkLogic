#include "asubpuretheorem.h"

using namespace N_Logic;

ASubPureTheorem::ASubPureTheorem(const std::string &name_, const N_Logic::IProposition::PropType type_):
    ASubTheorem(name_,type_)
{

}

ASubPureTheorem::ASubPureTheorem(const std::string &name_, const DbVar &dbVar, const IProposition::PropType type_):
    ASubTheorem(name_,dbVar,type_)
{

}

ASubPureTheorem::ASubPureTheorem(const std::string &name_, const DbVar &dbVar, const DbVar &dbVar2, const IProposition::PropType type_):
    ASubTheorem(name_,dbVar,dbVar2,type_)
{

}

ASubPureTheorem::ASubPureTheorem(const std::string &name_, const std::vector<DbVar> &dbVars, const IProposition::PropType type_):
    ASubTheorem(name_,dbVars,type_)
{

}

IISubTheoremFormula::TheoremFormulaType ASubPureTheorem::getFormulaType() const
{
    return SUBPURETH;
}
