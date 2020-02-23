#include "asubimpuretheorem.h"

using namespace N_Logic;

ASubImpureTheorem::ASubImpureTheorem(const std::string &name_, const PropType type_):
    ASubTheorem(name_,type_)
{

}

ASubImpureTheorem::ASubImpureTheorem(const std::string &name_, const DbVar &dbVar, const IProposition::PropType type_):
    ASubTheorem(name_,dbVar,type_)
{

}

ASubImpureTheorem::ASubImpureTheorem(const std::string &name_, const DbVar &dbVar, const DbVar &dbVar2, const IProposition::PropType type_):
    ASubTheorem(name_,dbVar,dbVar2,type_)
{

}

ASubImpureTheorem::ASubImpureTheorem(const std::string &name_, const std::vector<DbVar> &dbVars, const IProposition::PropType type_):
    ASubTheorem(name_,dbVars,type_)
{

}

IISubTheoremFormula::TheoremFormulaType ASubImpureTheorem::getFormulaType() const
{
    return SUBIMPURETH;
}
