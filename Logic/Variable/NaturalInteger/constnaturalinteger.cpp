#include "constnaturalinteger.h"

using namespace N_Logic;

ConstNaturalInteger::ConstNaturalInteger(const typename NaturalIntegerSet::Type &val)
    : Constante<NaturalIntegerSet>(val)
{

}

VALUE_TYPE ConstNaturalInteger::valueType() const
{
    return VALUE_TYPE::NATURAL_INT_TYPE;
}

std::string ConstNaturalInteger::toString(unsigned short) const
{
    return m_name;
}
