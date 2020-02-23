#include "naturalinteger.h"

using namespace N_Logic;

NaturalInteger::NaturalInteger(const std::string &name_): Variable<NaturalIntegerSet>(name_)
{

}

VALUE_TYPE NaturalInteger::valueType() const
{
    return VALUE_TYPE::NATURAL_INT_TYPE;
}

void NaturalInteger::operator=(const Variable::ValueType &val)
{
    m_val=std::make_unique<ValueType>(val);
}

NaturalInteger::~NaturalInteger()
{

}
