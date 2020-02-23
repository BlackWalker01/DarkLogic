#ifndef VALUETYPEOBJECT_H
#define VALUETYPEOBJECT_H
#include <memory>
#include <sstream>

namespace N_Logic {

enum VALUE_TYPE
{
    VOID_TYPE,
    BOOL_TYPE,
    NATURAL_INT_TYPE,
    BOOLEANSET_TYPE,
    NATURAL_INTEGERSET_TYPE
};

class ValueTypeObject
{
public:
    ValueTypeObject() = default;

    virtual VALUE_TYPE valueType() const=0;

    virtual ~ValueTypeObject() = default;
};
}
#endif // VALUETYPEOBJECT_H
