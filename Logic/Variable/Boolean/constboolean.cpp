#include "constboolean.h"

using namespace N_Logic;

ConstBoolean::ConstBoolean(const bool &val): Constante<BooleanSet>(val)
{

}

VALUE_TYPE ConstBoolean::valueType() const
{
    return VALUE_TYPE::BOOL_TYPE;
}

std::string ConstBoolean::toString(unsigned short) const
{
    return m_name;
}

template<>
std::string Constante<BooleanSet>::valToString(const bool& val)
{
    return val? "True" : "False";
}
