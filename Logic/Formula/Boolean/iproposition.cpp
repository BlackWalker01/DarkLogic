#include "iproposition.h"
#include "asubrule.h"
#include "asubtheorem.h"

using namespace N_Logic;

const std::string N_Logic::IProposition::s_HYP="HYP";

IProposition::IProposition(const std::string &name_, const IProposition::PropType type_):
    AbstractFormula<bool>(name_), m_type(type_)
{

}

IProposition::IProposition(const std::string &name_, const N_Logic::DbVar &dbVar,
                                        const N_Logic::IProposition::PropType type_):
    AbstractFormula<bool>(name_, dbVar), m_type(type_)
{

}

IProposition::IProposition(const std::string &name_, const N_Logic::DbVar &dbVar, const N_Logic::DbVar &dbVar2,
                           const N_Logic::IProposition::PropType type_):
    AbstractFormula<bool>(name_, dbVar, dbVar2), m_type(type_)
{

}

IProposition::IProposition(const std::string &name_, const std::vector<DbVar> &dbVars, const IProposition::PropType type_):
    AbstractFormula<bool>(name_, dbVars), m_type(type_)
{

}

IProposition::PropType N_Logic::IProposition::type() const
{
    return m_type;
}
