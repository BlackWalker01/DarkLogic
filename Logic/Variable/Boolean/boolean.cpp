#include "boolean.h"

using namespace N_Logic;

Boolean::Boolean(const std::string& name_): Variable<BooleanSet>(name_)
{

}

VALUE_TYPE Boolean::valueType() const
{
    return VALUE_TYPE::BOOL_TYPE;
}

void N_Logic::Boolean::operator=(const ValueType& val) const
{
    (*m_val) = std::make_unique<bool>(val);
}

Boolean::~Boolean()
{

}
