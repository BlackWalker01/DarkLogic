#include "constset.h"

using namespace N_Logic;

template<>
VALUE_TYPE ConstSet<NaturalIntegerSet>::valueType() const
{
    return NATURAL_INTEGERSET_TYPE;
}

template<>
VALUE_TYPE ConstSet<BooleanSet>::valueType() const
{
    return BOOLEANSET_TYPE;
}
