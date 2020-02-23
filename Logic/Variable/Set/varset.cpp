#include "varset.h"

using namespace N_Logic;

template<>
VALUE_TYPE VarSet<NaturalIntegerSet>::valueType() const
{
    return NATURAL_INTEGERSET_TYPE;
}

template<>
VALUE_TYPE VarSet<BooleanSet>::valueType() const
{
    return BOOLEANSET_TYPE;
}
